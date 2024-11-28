#!/usr/bin/env python
import os

libname = "spycardsonlinenative"

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp") + Glob("src/*/*.cpp") + Glob("src/*/*/*.cpp")

if env["target"] == "template_debug" and env["platform"] == "web":
    env.Append(CCFLAGS=["-gdwarf-4", "-g3"])
    env.Append(LINKFLAGS=["-gdwarf-4", "-g3"])

    project_path = Dir("..").abspath
    env.Append(CCFLAGS=[f"-ffile-prefix-map={project_path}=."])

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

file = "{}{}{}".format(libname, env["suffix"], env["SHLIBSUFFIX"])

if env["platform"] == "macos" or env["platform"] == "ios":
    platlibname = "{}.{}.{}".format(libname, env["platform"], env["target"])
    file = "{}.framework/{}".format(env["platform"], platlibname, platlibname)

libraryfile = "bin/{}/{}".format(env["platform"], file)
library = env.SharedLibrary(
    libraryfile,
    source=sources,
)

Default(library)
