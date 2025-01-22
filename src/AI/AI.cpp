#include "AI.h"

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
    if (img.channels == 4) {
        int numPixels = img.width * img.height;
        int rgbSize = numPixels * 3; // Each pixel will have 3 components: R, G, B

        // Allocate memory for the RGB image
        unsigned char* rgbData = new unsigned char[rgbSize];

        for (int i = 0; i < numPixels; ++i) {
            rgbData[i * 3 + 0] = img.data[i * 4 + 0];
            rgbData[i * 3 + 1] = img.data[i * 4 + 1];
            rgbData[i * 3 + 2] = img.data[i * 4 + 2];
        }
        img.data = rgbData;
        img.channels = 3;
    }

    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "ONNXRuntime");
    Ort::SessionOptions session_options;
    Ort::Session session(env, L"C:/Users/ronit/Downloads/4xSPANkendata_fp32.onnx", session_options);

    int img_size = img.width * img.height * img.channels;
    float* preprocessed_data = new float[img_size];

    for (int i = 0; i < img_size; ++i) {
        preprocessed_data[i] = img.data[i] / 255.0f; // Normalize to [0, 1]
    }

    const int64_t input_shape[] = {1, img.channels, img.height, img.width}; // Batch size 1, Channels, Height, Width
    size_t input_shape_len = sizeof(input_shape) / sizeof(input_shape[0]);

    // Create memory info for the tensor
    OrtMemoryInfo* memory_info = nullptr;
    Ort::GetApi().CreateCpuMemoryInfo(OrtArenaAllocator, OrtMemTypeDefault, &memory_info);

    // Create tensor for input
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, preprocessed_data, img.width * img.height * img.channels, input_shape, input_shape_len);

    std::vector<Ort::Value> output_tensors; // Assuming a single output tensor
    const int64_t output_shape[] = {1, img.channels, img.height * 4, img.width * 4};
    size_t output_shape_len = sizeof(output_shape) / sizeof(output_shape[0]);

    // Run inference
    std::vector<const char*> input_names = {"input"}; // The name of the input
    std::vector<const char*> output_names = {"output"}; // The name of the output

    output_tensors.push_back(Ort::Value::CreateTensor<float>(memory_info, new float[(img.width * 4) * (img.height * 4) * img.channels], (img.width * 4) * (img.height * 4) * img.channels, output_shape, output_shape_len));

    // Use the correct Run method that accepts input and output counts
    session.Run(Ort::RunOptions(), input_names.data(), &input_tensor, 1, output_names.data(), output_tensors.data(), 1);

    // Postprocess the output
    float* result_data = (float*)output_tensors[0].GetTensorData<float>();
    const int upscaled_width = output_tensors[0].GetTensorTypeAndShapeInfo().GetShape()[3];
    const int upscaled_height = output_tensors[0].GetTensorTypeAndShapeInfo().GetShape()[2];
    const int upscaled_size = upscaled_width * upscaled_height * img.channels;
    unsigned char* output_data = nullptr;
    output_data = new unsigned char[upscaled_size];

    for (int i = 0; i < upscaled_size; ++i) {
        output_data[i] = result_data[i] * 255;
    }

    output = Image(upscaled_width, upscaled_height, img.channels, output_data); //df
    completed = true;
}