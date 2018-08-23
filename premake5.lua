-- http://industriousone.com/scripting-reference

local action = _ACTION or ""
local OPENCV_PATH = "d:/opencv/build"

solution "opencv-cookbook"
    location (action)
    configurations { "Debug", "Release" }
    platforms {"x64"}
    language "C++"

    os.mkdir("bin")
    targetdir ("bin")

    filter "action:vs*"
        defines { "_CRT_SECURE_NO_WARNINGS" }
        if not os.isfile("bin/opencv_world340d.dll") then
            os.copyfile(path.join(OPENCV_PATH, "x64/vc14/bin/opencv_world340d.dll"), "bin/opencv_world340d.dll")
            os.copyfile(path.join(OPENCV_PATH, "x64/vc14/bin/opencv_world340.dll"), "bin/opencv_world340.dll")
            os.copyfile(path.join(OPENCV_PATH, "x64/vc14/bin/opencv_ffmpeg340_64.dll"), "bin/opencv_ffmpeg340_64.dll")
        end

    filter "system:macosx"
        defines {
            "_MACOSX",
        }

    configuration "Debug"
        defines { "DEBUG" }
        symbols "On"
        targetsuffix "-d"

    configuration "Release"
        defines { "NDEBUG" }
        flags { "No64BitChecks" }
        editandcontinue "Off"
        optimize "Speed"
        optimize "On"
        editandcontinue "Off"

    function create_example_project( example )
        local proj = example[1]
        project (proj)
            kind "ConsoleApp"
            files {
                {table.unpack(example, 2, 8)}
            }

            includedirs { 
                path.join(OPENCV_PATH, "include")
            }

            libdirs {
                path.join(OPENCV_PATH, "x64/vc14/lib"),
            }

            configuration "Debug"
                links {
                    "opencv_world340d.lib",
                }
            
            configuration "Release"
                links {
                    "opencv_world340.lib",
                }
    end

    local examples = {
        {"01-main1", "Chapter 01/main1.cpp"},
        {"01-main2", "Chapter 01/main2.cpp"},
        {"02-addImages", "Chapter 02/addImages.cpp"},
        {"02-colorReduce", "Chapter 02/colorReduce.cpp"},
        {"02-contrast", "Chapter 02/contrast.cpp"},
        {"02-saltImage", "Chapter 02/saltImage.cpp"},
        {"03-colorDetection", "Chapter 03/colorDetection.cpp", "Chapter 03/colordetector.cpp"},
        {"04-finder", "Chapter 04/finder.cpp"},
        {"04-histograms", "Chapter 04/histograms.cpp"},
        {"04-objectfinder", "Chapter 04/objectfinder.cpp"},
        {"04-retrieve", "Chapter 04/retrieve.cpp"},
        {"05-morpho2", "Chapter 05/morpho2.cpp"},
        {"05-morphology", "Chapter 05/morphology.cpp"},
        {"05-segment", "Chapter 05/segment.cpp"},
        {"06-derivatives", "Chapter 06/derivatives.cpp"},
        {"06-filters", "Chapter 06/filters.cpp"},
        {"07-blobs", "Chapter 07/blobs.cpp"},
        {"07-contours", "Chapter 07/contours.cpp"},
        {"08-interestPoints", "Chapter 08/interestPoints.cpp"},
        {"08-tracking", "Chapter 08/tracking.cpp"},
        {"09-calibrate", "Chapter 09/calibrate.cpp", "Chapter 09/CameraCalibrator.cpp"},
        {"09-estimateF", "Chapter 09/estimateF.cpp"},
        {"09-estimateH", "Chapter 09/estimateH.cpp"},
        {"09-robustmatching", "Chapter 09/robustmatching.cpp"},
        {"10-foreground", "Chapter 10/foreground.cpp"},
        {"10-tracking", "Chapter 10/tracking.cpp"},
        {"10-videoprocessing", "Chapter 10/videoprocessing.cpp"},
    }
    for _, example in ipairs(examples) do
        create_example_project(example)
    end

