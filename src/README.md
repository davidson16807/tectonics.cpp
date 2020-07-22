This folder contains source code that's relevant only to the tectonics.cpp application. 

It contains only logic that's relevant to the view and update components of this application's modified "Model/View/Update" architecture. See [here](https://guide.elm-lang.org/architecture/) for a introduction to the architecture. 

This folder does not contain model logic, since that logic can be reused across many different applications. For model logic, see `inc/academics`, `inc/components`, and `inc/systems`. 

Some functionality requires use of the `glfw` library. 