# Shunting-yard Calculator

> The Shunting-yard Calculator is written in C++ by [@TylerJenningsW](https://github.com/TylerJenningsW) and [@MichaelLillis](https://github.com/MichaelLillis) using the [Shunting-yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) and [wxWidgets](https://www.wxwidgets.org/) as the primary interface library. 

>*NOTE:* This project is meant to be a learning experience, as both authors had limited knowledge of C++ and the wxWidgets library going into the project.


<img src="https://user-images.githubusercontent.com/36655595/199161106-53c15023-eb94-423f-9e46-41289c02e22b.png" img align="right" width=35%>

## General Information ℹ️

- Supports multiplication by asterisk, multiplication by parentheses, division, addition, subtraction, SIN, COS, TAN, and exponents.
- Set the amount of decimal places to round to using the config.txt

## Installation 💻

- Download and install [wxWidgets](https://www.wxwidgets.org/downloads/)
- Move your wxWidgets folder to your C drive
- Rename the folder to wxWidgets
- Navigate to C:\wxWidgets\build\msw
- Open wx_vc17.sln using Visual Studio
- Set the Solution platform to be x64
- In the solution explorer, left click on the _custom_build project. Next, scroll down to the xrc project, and shift + left-click on it. At this point, everything in the solution explorer, except the solution, should be highlighted. Next, right click any of the highlighted projects, and choose properties.
- Under Configuration properties, click advanced
- For the character set property, set it to Not Set, and click ok.
- In the build menu, click build solution, you may need to do this more than once

## Algorithm Explaination 🎥


## Screenshots/Examples 📷

#### Layout
![Calculator_layout](https://user-images.githubusercontent.com/95639661/199864717-ee511609-4e51-4fc2-aaec-1663e7c90489.png)


#### Results
![calc](https://user-images.githubusercontent.com/95639661/199865552-5cbfe82c-eeef-4a2d-81f2-4d42a5a83dac.gif)


## Links 🔗

> [wxWidgets - Library](https://www.wxwidgets.org/) ![development-wxwidgets](https://user-images.githubusercontent.com/95639661/199485724-1726d764-b71b-41eb-ad08-a7f80b6a52a2.png)

