# Undo Buffers with Infinite History Recall
* Small design exercise implementing undo buffers with infinite history recall.
* See the accompanying [blog post](http://chillusweblog.com/2018/10/29/designing-undo-buffers/) for a nicer walkthrough of the code in this repository.

# Dependencies
You need Bazel and C++14 to run this code.
<pre>
$ bazel run :main
INFO: Build options have changed, discarding analysis cache.
INFO: Analysed target //:main (0 packages loaded).
INFO: Found 1 target...
Target //:main up-to-date:
  bazel-bin/main
INFO: Elapsed time: 1.018s, Critical Path: 0.80s
INFO: 4 processes: 4 darwin-sandbox.
INFO: Build completed successfully, 7 total actions
INFO: Build completed successfully, 7 total actions
INITIAL STATE: I have a blue SUV that fits 4.
CURRENT STATE: I have a blue SUV that fits 4.
CURRENT STATE: I have a blue SUV that fits 
CURRENT STATE: I have a blue SUV that fits 10!
CURRENT STATE: I have a blue
CURRENT STATE: I have a 
CURRENT STATE: I have a red car.
CURRENT STATE: I have a 
CURRENT STATE: I have a blue
FINAL STATE  : 
</pre>
