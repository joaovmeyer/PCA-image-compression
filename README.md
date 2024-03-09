# PCA image compression

The idea is to use PCA to reduce the image size, loosing minimum quality. We build a dataset where every row of the image is a data point, and aply PCA to it. This will effectively reduce the amount of columns in our image while maintaining the maximum range of representable rows in the original space, giving us good compression without too much loss in image quality.

![image](https://github.com/joaovmeyer/PCA-image-compression/assets/144701021/dfb65a0c-115b-401c-9a50-3032caa56fdd)
