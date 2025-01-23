#include "AI.h"

std::string getTempFolderPath() {
#ifdef _WIN32
    char tempPath[MAX_PATH];
    if (GetTempPathA(MAX_PATH, tempPath)) {
        return std::string(tempPath);
    }
#else
    const char* tmpDir = std::getenv("TMPDIR");
    if (tmpDir) {
        return std::string(tmpDir);
    } else {
        return "/tmp"; // default if TMPDIR is not set
    }
#endif
    return "";
}

void UpscaleNode::execute() {
    Image img;
    std::string modelPath;
    for (auto s : dependencies) {
        if (s.first == "image" && !nodes[s.second]->completed) {
            nodes[s.second]->execute();
            img = nodes[s.second]->output;
        }
    }
    for (auto s : params) {
        if (s.first == "model") {
            modelPath = s.second;
        }
    }

    if (img.channels < 3) {
        std::cout << "[Composite::Render] Error: Image must have at least 3 channels.\n";
    }

    std::string tempFolderPath = getTempFolderPath();

    if (tempFolderPath.empty()) {
        std::cerr << "Failed to get temp folder path!" << std::endl;
    }
    
    std::string filePath = tempFolderPath + "/temp.png";
    std::string filePathOut = tempFolderPath + "/temp_upscaled.png";

    if (!stbi_write_png(filePath.c_str(), img.width, img.height, img.channels, img.data, img.width * img.channels)) {
        std::cerr << "Failed to save the image!" << std::endl;
    }

    Py_Initialize();

    PyGILState_STATE gstate = PyGILState_Ensure();


    PyObject* pName = PyUnicode_DecodeFSDefault("ex");
    PyObject* pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != nullptr) {
        PyObject* pFunc = PyObject_GetAttrString(pModule, "main");

        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject* pArgs = PyTuple_Pack(3,
                PyUnicode_FromString(modelPath.c_str()),
                PyUnicode_FromString(filePath.c_str()),
                PyUnicode_FromString(filePathOut.c_str())
            );

            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

            if (pValue != nullptr) {
                Py_XDECREF(pValue);
            } else {
                PyErr_Print();
            }

            Py_XDECREF(pArgs);
            Py_XDECREF(pFunc);
        } else {
            PyErr_Print();
        }

        Py_XDECREF(pModule);
    } else {
        PyErr_Print();
    }

    Py_Finalize();

    int width, height, channels;
    unsigned char* image = stbi_load(filePathOut.c_str(), &width, &height, &channels, 0);

    output = Image(width, height, channels, image);
    completed = true;
}