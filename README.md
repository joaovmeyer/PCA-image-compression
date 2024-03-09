# PCA image compression

The idea is to use PCA to reduce the image size, loosing minimum quality. We build a dataset where every row of the image is a data point, and aply PCA to it. This will effectively reduce the amount of columns in our image while maintaining the maximum range of representable rows in the original space, giving us good compression without too much loss in image quality.

![7475467854785](https://github.com/joaovmeyer/PCA-image-compression/assets/144701021/6cf41455-3584-4e0d-9549-fdf48dd4a82a)
