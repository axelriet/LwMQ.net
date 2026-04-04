This folder contains user-contributed sample projects for LwMQ.

Create an appropriate subfolder (e.g. 'Messaging' for a user-contributed messaging sample) if needed and put the *project* file there, along with the files the project is made of.

Look at the official samples for the typical layout.

Then, add your project to the main solution, creating a solution folder that matches your new sample's folder as necessary.

Say your new messaging sample is named PingPong.

0) Create a branch with git checkout -b PingPong
1) Create .\LwMQ.net\samples\User-Contributions\Messaging\PingPong
2) Put PingPong.cvxproj etc in that folder.
3) In the LwMQ.Samples solution, under User-Contributions, create a new 'Messaging' folder (right-click User-Contributions, Add, New Solution Folder).
4) Add your project to that solution folder (right-click, Add, Existing Project...)

No particular coding convention (or language) is required for user-contributed samples,
but sure everything builds directly from the IDE with zero warnings!

At the end, the folders on disk looks like 

    .\LwMQ.net\samples\User-Contributions\Messaging\PingPong

and the solution tree looks like that:

    LwMQ.Samples
        User-Contributions
            Messaging
                PingPong

If you create the project from the IDE (right-click User-Contributions->Messaging) then make sure the Location: is correct, e.g.

    D:\GIT\LwMQ.net\samples\User-Contributions\Messaging\

    and then name your project appropriatly (e.g. PingPong)

Once evything looks good and your new sample builds cleanly and runs fine, commit your files and send a PR.

Happy coding!!!
