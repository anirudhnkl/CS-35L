Name: Anirudh Balasubramaniam
UID:  004906517

Implementation and Issues

I realized that I had to split up the work done by the rendering code in the main method. I was able to do this with the pthread library, specifically
pthread_create and pthread_join. I moved all the code that did the rendering work into a separate function. But I had to figure out how to split up the work.
I did this by breaking the image into parts and certain threads would work on certain parts of the image. Another was printing of the color values and I was able
to solve this by creating a 3D array that stored the values and printed them after all the threads are finished.

Conclusion

The performance of SRT clearly improved with the implementation of multithreading, as can be seen with the time commands. While the user time remained somewhat
constant, the real time decreased proportionally to the increase in number of threads. By parallelizing the work, we were able to reduce the time taken to
create the image.

time ./srt 1-test.ppm >1-test.ppm.tmp

real    0m47.896s
user    0m47.886s
sys     0m0.002s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real    0m23.967s
user    0m47.678s
sys     0m0.002s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real    0m12.543s
user    0m49.118s
sys     0m0.003s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real    0m6.901s
user    0m53.357s
sys     0m0.009s