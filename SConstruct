#!/usr/bin/env python
import sys

if sys.version_info.major < 3:
    raise Exception("Python 3 is required, bailing out")
if sys.version_info.minor < 5:
    raise Exception(
        "Python 3.5 is the minimum supported Python version, bailing out")

from pathlib import Path

EXTENSION_NAME: str = "spout_gd"
OUTPUT_DIR: str = "out/"

Path(OUTPUT_DIR).mkdir(exist_ok=True)

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["src", "."])

src_directory: Path = Path("src/")
sources: list = [Glob("src/*.cpp")]  # Need to add all files in src/ first

for path_obj in src_directory.glob("**/*"):
    if path_obj.is_dir():
        sources += Glob("{}/*.cpp".format(str(path_obj)))

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "{}/{}.{}.{}.framework/{}.{}.{}".format(
            OUTPUT_DIR, EXTENSION_NAME, env["platform"], env["target"],
            EXTENSION_NAME, env["platform"], env["target"],
        ),
        source=sources
    )
else:
    library = env.SharedLibrary(
        "{}/{}.{}.{}{}".format(
            OUTPUT_DIR,
            EXTENSION_NAME, env["platform"], env["target"], env["SHLIBSUFFIX"]
        ),
        source=sources
    )

Default(library)
