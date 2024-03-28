About project

I created this to improve certain skills - for example I tried to implement inheritance, also tried to play sets, maps and sfml in general. I didnt try to do everything the most efficient way. Sometimes I even experimented with few things just to see if they would work. 

It is also my first bigger project so if I would write this again, I would organize the code a bit differently. 

The project is not finished, because it took me a bit longer, then I expected and I want to have something that I can show right now. However I think it presentable at this point.

---------------------------------------------------------------------------

How to run

Project uses SFML library, I think you should be able to run it if you pull master branch and then just debug it at your PC (while having SFML properly installed).

I installed smfl with just vcpkg, however if I remember correctly it is not neccessary, you only need to dowload here https://www.sfml-dev.org/download.php and then copy dll files into folder, where your program is executed (if you execute it with visual studio then next to the .sln file, or when you run app, the dll files need to be next to the app).

I dont know if this is my problem or everyones, but I can not debug the same code twice with sfml libary. When I do, it tells me that some dll files are missing. To fix the problem, add comment anywhere in code (using //) to make it a bit different. Then you should be able to debug again.
