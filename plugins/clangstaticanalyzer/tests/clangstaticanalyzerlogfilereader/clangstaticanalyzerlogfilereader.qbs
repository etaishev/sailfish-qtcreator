import qbs
import "../clangstaticanalyzerautotest.qbs" as ClangStaticAnalyzerAutotest

ClangStaticAnalyzerAutotest {
    name: "ClangStaticAnalyzerLogFileReader Autotest"

    Group {
        name: "sources from plugin"
        prefix: pluginDir + '/'
        files: [
            "clangstaticanalyzerdiagnostic.cpp",
            "clangstaticanalyzerdiagnostic.h",
            "clangstaticanalyzerlogfilereader.cpp",
            "clangstaticanalyzerlogfilereader.h",
        ]
    }

    files: [
        "tst_clangstaticanalyzerlogfilereader.cpp"
    ]
}
