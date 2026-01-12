<a name="readme-top"></a>

<!-- PROJECT SHIELDS
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]
 -->


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="#">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Dear ImXML</h3>

  <p align="center">
	A lightweight XML to ImGui library
    <br />
    <!--<a href="https://github.com/othneildrew/Best-README-Template"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template">View Demo</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Report Bug</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Request Feature</a>-->
  </p>
</div>



<!-- TABLE OF CONTENTS 
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>
-->


<!-- ABOUT THE PROJECT -->
## About The Project

Dear ImXML allows you to writer ImGui interfaces in an XML format and render / interact with them at runtime.

For example this XML file
```XML
<?xml version="1.0" encoding="UTF-8"?>

<begin name="Title window" flags="ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse">
	<menubar>
		<menu label="File">
			<menuitem label="New..."/>
			<menu label="Open...">
				<menuitem label="File1.txt" />
			</menu>
		</menu>
		<menu label="Edit">
			<menuitem label="Foo"/>
			<menuitem label="Bar"/>
		</menu>
	</menubar>
	<table name="table0" columns="2">
		<row>
			<column>
				<text label="Text"/>
				<button id="btn0" label="Hello!"/>
				<text label="new line"/>
				<sliderfloat label="float" dynamic="float0" min="0" max="1" />
				<inputtext label="string" dynamic="str0"/>
				<placeholder id="custom0"/>
			</column>
			<column>
				<treenode label="treeroot">
					<treenode label="tree0" />
					<treenode label="tree1">
						<treenode label="tree2" />
					</treenode>
				</treenode>
				<group>
					<colorpicker3 dynamic="color0" />
				</group>
				<separator/>
				<group>
					<PopupContextWindow>
						<text label="context popup"/>
					</PopupContextWindow>
					<text label="right click to open popup"/>
				</group>
			</column>
		</row>
	</table>
</begin>
```

Turns into this window
![Demo gif](images/demo.gif)

Note that the project is at an early phase, not all ImGui elements are supported but it's rapidly being expanded.
<!--[![Product Name Screen Shot][product-screenshot]](https://example.com)-->




<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white) ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) 


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

You will need CMake

### Installation

If you use CMake you can simply do
```cmake
message(STATUS "Fetching DearImXML")
include(FetchContent)
FetchContent_Declare(
  DearImXML
  GIT_REPOSITORY https://github.com/ProtectedVariable/DearImXML.git
  GIT_TAG master
)
FetchContent_MakeAvailable(DearImXML)
```

And then 
`target_link_libraries(${PROJECT_NAME} PUBLIC DearImXML)`

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Usage

Check out the `example` folder of the repo for a quick example.

You can also check out https://github.com/ProtectedVariable/ICE for a complete application using this library.

<p align="right">(<a href="#readme-top">back to top</a>)</p>
 


<!-- ROADMAP 
## Roadmap

- [x] Add Changelog
- [x] Add back to top links
- [ ] Add Additional Templates w/ Examples
- [ ] Add "components" document to easily copy & paste sections of the readme
- [ ] Multi-language Support
    - [ ] Chinese
    - [ ] Spanish

See the [open issues](https://github.com/othneildrew/Best-README-Template/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

-->
<!-- CONTRIBUTING -->
## Contributing

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request


<p align="right">(<a href="#readme-top">back to top</a>)</p>



## License

Distributed under the LGPLv2.1 License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Feel free to tag me in an issue :).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [DearImGui](https://github.com/ocornut/imgui/)
* [Choose an Open Source License](https://choosealicense.com)
* [Markdown Badges](https://ileriayo.github.io/markdown-badges/)
* [Awesome Readme](https://github.com/matiassingers/awesome-readme)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
