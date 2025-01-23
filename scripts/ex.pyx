import onnxruntime as ort
import numpy as np
from PIL import Image

def load_image(image_path):
    """ Load image and convert to RGB """
    img = Image.open(image_path).convert('RGB')
    return np.array(img)

def preprocess_image(image):
    """ Preprocess image (normalize and reshape) for ONNX model input """
    # Normalize image to [0, 1]
    image = image.astype(np.float32) / 255.0
    
    # Add batch dimension (model expects batch size as the first dimension)
    image = np.expand_dims(image, axis=0)
    
    # Transpose image from HWC to CHW format (model expects channel-first format)
    image = np.transpose(image, (0, 3, 1, 2))
    
    return image

def postprocess_image(output):
    """ Postprocess the model's output (convert back to image) """
    # Remove batch dimension and transpose back to HWC
    output = np.squeeze(output, axis=0)
    output = np.transpose(output, (1, 2, 0))
    
    # Convert to 0-255 range
    output = (output * 255.0).clip(0, 255).astype(np.uint8)
    
    return Image.fromarray(output)

def upscale_image(model_path, image_path, output_path):
    """ Load ONNX model and perform image upscaling """
    # Load ONNX Runtime session
    session = ort.InferenceSession(model_path)

    # Load and preprocess the image
    image = load_image(image_path)
    preprocessed_image = preprocess_image(image)

    # Run the model
    input_name = session.get_inputs()[0].name
    output_name = session.get_outputs()[0].name
    result = session.run([output_name], {input_name: preprocessed_image})

    # Postprocess the result and save the upscaled image
    upscaled_image = postprocess_image(result[0])
    upscaled_image.save(output_path)
    print(f"Upscaled image saved to {output_path}")

# Example usage
def main(model_path, image_path, output_path):
    upscale_image(model_path, image_path, output_path)
