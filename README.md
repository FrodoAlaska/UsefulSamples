## What Is This?

Often, when I was using open-source libraries for game development, I had a problem finding a good example/sample of the library in use. Usually, the library's repo would not include a cohesive example that shows the library's usage in a _pure_ form. Some libraries did not even have examples. I wanted to make a public repository of a bunch of _pure_ examples. Examples that do not include any junk. Just the library in question being used properly. The examples will not include OpenGL logic or command line argument parsing. Neither will it be part of a bigger project with a bunch of external code influencing it. Just a single `main.cpp` file with an example of the library being used. 

## How To Use This? 

All the code here is free and can be copied in whatever way you like. The examples will not contain any build options or library linkage, though. It's just purely code. You can, however, take it and compile it in your own project. The code is guaranteed to be compiled in its current form after some changes, such as file paths, includes, defines, and whatnot.

At the top of each `main.cpp` file, you'll find a link to the fantastic library that was used. Please go to these libraries and give them all the love. They truly deserve every bit of attention they can get. 

## Contribution

If you wish to include a library that you've been using and you don't see it here, or you wish to change something that I might have missed, you are _very_ welcome to do so. This is for the community, after all. However, there are a few "rules" that you have to follow. They are not strict by any means. But in order to keep the informative nature of this repo, you need to follow the few rules below.

1. **Please make sure to comment your code and what each function and/or type does**. If there is a parameter that is complicated to understand or is not obvious from the library's docs, make sure to explain it. This is not a tutorial, remember. So you do not need to explain how textures or fonts work. But just briefly go over what each function does, how to use it, and when to use it. 

2. **Make sure your code is formatted properly**. You do not have to follow any conventions. Code the way you like to code, man. Just make sure that there are spaces between each line and the variables aren't just `int a` or whatever. And, overall, the code is cohesive and clean. Again, this is supposed to be informative. 

3. **Do not include any external library that is not concerned with the library in question**. If you're making an example for `stb_truetype`, for example, do not include OpenGL code that creates a texture. That is not the point of the example. Show the _usage_ of `stb_truetype` on its own. Not with another library. 

4. **Do not add any unnecessary junk**. If you're planning to use OpenAL for audio, do not try to use the command line arguments to get the audio file's path, for instance, or to set the channels of the audio samples. Do not create a `class` that represents the audio file. That's unnecessary. Just show how OpenAL is to be used. That's it. No extra junk that is unrelated to the example.  

5. **Make sure that the code works**. It doesn't have to compile, but make sure the _usage_ is correct and not flawed. You don't have to be strict about the semantics, but make sure that the code is actually valid and does what it's supposed to do. So don't misuse a certain function or a certain type.

And finally, I'm welcome to any feedback on the project. This is not a rule, but more of a favor. If you have any ideas or opinions about certain things in the repo, please send me a message and I'll get back to you as soon as I can. Once again, this is for the community. Any suggestions are most welcome.
