#!/usr/bin/env python
import os

libname = "spycardsonlinenative"

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["src/", "godot4-squirrel/src/"])
sources = Glob("src/*.cpp") + Glob("src/*/*.cpp") + Glob("src/*/*/*.cpp")

sources += SConscript("godot4-squirrel/SCsub", {"env": env})

if env["target"] == "template_debug":
    if env.get("is_msvc", False):
        env.Append(CXXFLAGS=["/WX"])
    else:
        env.Append(CXXFLAGS=["-Werror"])
        if env["CXXVERSION"].startswith("13."):
            env.Append(CXXFLAGS=["-Wno-attribute"])

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
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
