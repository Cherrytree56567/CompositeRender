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

void BackgroundRemoverNode::execute() {
    Image img;
    std::string modelPath;
    for (auto s : dependencies) {
        if (s.first == "image" && !nodes[s.second]->completed) {
            nodes[s.second]->execute();
            img = nodes[s.second]->output;
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
    std::string filePathOut = tempFolderPath + "/temp_bged.png";

    if (!stbi_write_png(filePath.c_str(), img.width, img.height, img.channels, img.data, img.width * img.channels)) {
        std::cerr << "Failed to save the image!" << std::endl;
    }

    Py_Initialize();

    PyGILState_STATE gstate = PyGILState_Ensure();


    PyObject* pName = PyUnicode_DecodeFSDefault("ex");
    PyObject* pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != nullptr) {
        PyObject* pFunc = PyObject_GetAttrString(pModule, "remove_background");

        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject* pArgs = PyTuple_Pack(2,
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

void ImageGenNode::execute() {
    std::string prompt;
    bool cuda;
    std::string model;
    for (auto s : params) {
        if (s.first == "prompt") {
            prompt = s.second;
        }
        if (s.first == "cuda") {
            if (s.second == "true") {
                cuda = true;
            }
            if (s.second == "false") {
                cuda = false;
            }
        }
        if (s.first == "model") {
            model = s.second;
        }
    }

    std::string tempFolderPath = getTempFolderPath();

    if (tempFolderPath.empty()) {
        std::cerr << "Failed to get temp folder path!" << std::endl;
    }
    
    std::string filePathOut = tempFolderPath + "/temp_generated.png";

    Py_Initialize();

    PyGILState_STATE gstate = PyGILState_Ensure();


    PyObject* pName = PyUnicode_DecodeFSDefault("ex");
    PyObject* pModule = PyImport_Import(pName);
    Py_XDECREF(pName);

    if (pModule != nullptr) {
        PyObject* pFunc = PyObject_GetAttrString(pModule, "aiImage");

        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject* pArgs = PyTuple_Pack(4,
                PyBool_FromLong(cuda),
                PyUnicode_FromString(prompt.c_str()),
                PyUnicode_FromString(filePathOut.c_str()),
                PyUnicode_FromString(model.c_str())
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

void HDRNode::execute() {
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
        PyObject* pFunc = PyObject_GetAttrString(pModule, "AI_HDR");

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