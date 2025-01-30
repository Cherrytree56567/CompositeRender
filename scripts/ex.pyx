import onnxruntime as ort
import numpy as np
from PIL import Image
from rembg import remove
from diffusers import StableDiffusionPipeline
import torch
import tensorflow as tf
import cv2
import numpy as np

def AI_HDR(model, image, output):
    model = tf.keras.models.load_model(model)
    model.summary()

    image = cv2.imread(image)
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    image_rgb = image_rgb / 255.0
    image_resized = cv2.resize(image_rgb, (256, 256))
    image_input = np.expand_dims(image_resized, axis=0)
    enhanced_image = model.predict(image_input)
    enhanced_image = np.squeeze(enhanced_image, axis=0)
    enhanced_image = np.clip(enhanced_image, 0, 1)
    enhanced_image = (enhanced_image * 255).astype(np.uint8)
    enhanced_image_bgr = cv2.cvtColor(enhanced_image, cv2.COLOR_RGB2BGR)
    cv2.imwrite(output, enhanced_image_bgr)

def aiImage(cuda, prompt, output, model):
    model = StableDiffusionPipeline.from_pretrained(model)
    if cuda:
        model.to("cuda")

    image = model(prompt).images[0]

    image.save(output)


def remove_background(input_path, output_path):
    # Open the input image
    with open(input_path, 'rb') as input_file:
        input_image = input_file.read()
    
    # Remove the background
    output_image = remove(input_image)
    
    # Save the output image
    with open(output_path, 'wb') as output_file:
        output_file.write(output_image)

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
