# Assignment 3



* All the interpolators output the correct values for the test.
* The curve editor works perfectly except for deletion:
*   Since the keys are not updated when the control points are visible per the UI code,to make sure no index out of bounds errors occur, I show the keys while in delete mode. Since the keys are not updated, the old keys are shown.

https://user-images.githubusercontent.com/43979389/135680000-6d4e365b-43f4-4089-b0f1-e372d0ce3cee.mov


* Uses Hemite interpolator to randomize particle movement, and catmull-rom interpolation for color.

https://user-images.githubusercontent.com/43979389/135680009-027c8aa1-b3da-4735-a4dc-50c213dc81a3.mov

