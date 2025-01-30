import tensorflow as tf

# Specify the checkpoint directory
checkpoint_dir = 'pretrained_ckp/'

# Define your model architecture here (same as the one used during training)
model = tf.keras.Sequential([
    tf.keras.layers.InputLayer(input_shape=(256, 256, 3)),  # Adjust input shape as necessary
    tf.keras.layers.Conv2D(32, kernel_size=3, activation='relu'),
    # Add other layers as per your model architecture
])

# Restore the model weights from the checkpoint
checkpoint = tf.train.Checkpoint(model=model)
checkpoint.restore(checkpoint_dir + 'pretrained').expect_partial()

print("Model loaded successfully!")

# Save the model as a SavedModel
saved_model_dir = 'saved_model/'
model.save("saved.h5")

print(f"Model saved at {saved_model_dir}")
