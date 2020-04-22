import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout, Activation, Flatten, Conv2D, MaxPooling2D
from tensorflow.keras.callbacks import TensorBoard
import pickle
import time
import tensorflow.keras

NAME = "Blueprint-Image-Classifier-cnn-64x2-{}".format(int(time.time()))

# Create callback board
tensorboard = TensorBoard(log_dir='logs\\{}'.format(NAME))

pickle_in = open("X.pickle", "rb")
X = pickle.load(pickle_in)

pickle_in = open("Y.pickle", "rb")
Y = pickle.load(pickle_in)
Y = tf.keras.utils.to_categorical(Y, 3)

dense_layers = [1]
dense_layer_sizes = [64]
conv_layers = [2]
conv_layer_sizes = [32]
kernal_sizes = [3]
num_epochs = [10]

for dense_layer in dense_layers:
    for dense_layer_size in dense_layer_sizes:
        for conv_layer in conv_layers:
            for conv_layer_size in conv_layer_sizes:
                for kernal_size in kernal_sizes:
                    for num_epoch in num_epochs:
                        NAME = "{}-conv-{}-nodes-{}-dense-{}-kernal-{}-numepoch-{}".format(conv_layer, dense_layer_size, dense_layer, kernal_size, num_epoch, int(time.time()))
                        print(NAME)

                        model = Sequential()

                        model.add(Conv2D(conv_layer_size, (kernal_size, kernal_size), input_shape=X.shape[1:]))
                        model.add(Activation('relu'))
                        model.add(MaxPooling2D(pool_size=(2, 2)))

                        for l in range(conv_layer-1):
                            model.add(Conv2D(conv_layer_size, (kernal_size, kernal_size)))
                            model.add(Activation('relu'))
                            model.add(MaxPooling2D(pool_size=(2, 2)))


                        model.add(Flatten())

                        for _ in range(dense_layer):
                            model.add(Dense(dense_layer_size))
                            model.add(Activation('relu'))


                        model.add(Dense(3))
                        model.add(Activation('softmax'))

                        tensorboard = TensorBoard(log_dir="logs\\{}".format(NAME))

                        model.compile(loss='categorical_crossentropy',
                                      optimizer='adam',
                                      metrics=['accuracy'])

                        model.fit(X, Y,
                                  batch_size=32,
                                  # steps_per_epoch=50,
                                  # validation_steps=25,
                                  epochs=num_epoch,
                                  validation_split=0.3,
                                  callbacks=[tensorboard],
                                  shuffle=True)

model.save('CNN.model')

