<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
<div align="center">

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]

</div>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/Pip-Install-Party/Interpreter">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">Interpreter</h3>

  <p align="center">
    This is an interpreter for the C language. 
    <br />
    <br />
    <a href="https://github.com/Pip-Install-Party/Interpreter/blob/main/README.md"><strong>Explore the docs »</strong></a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
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



<!-- ABOUT THE PROJECT -->
## About The Project

This program was completed as a final project among five students for CS460 (Programming Languages) at Sonoma State University in fall 2024. 

<!-- GETTING STARTED -->
<a id="readme-getting-started"></a>
## Getting Started

The interpreter is compatible with Windows, MacOS, and Linux. 

### Running The Program

### MacOS / Linux 
  1. Clone the repo
   ```sh
   git clone https://github.com/Pip-Install-Party/Interpreter.git
   ```
  2. Run the makefile
  ```sh
   make
   ```
  3. Run the executable
  ```sh
   ./interpreter.x
  ```

### Windows 
 1. Clone the repo
   ```sh
   git clone https://github.com/Pip-Install-Party/Interpreter.git
   ```
  2. Run the makefile
  ```sh
   make
   ```
  3. Run the executable
  ```sh
   ./interpreter.exe
  ```
<!-- ROADMAP -->
## Roadmap

This project was implemented in stages. Each state builds from the last and ultimately constructs an interpreter. 

1️⃣ - [Comment Removal](https://github.com/Pip-Install-Party/Ignoring-Comments)  
2️⃣ - [Tokenization](https://github.com/Pip-Install-Party/Tokenization)  
3️⃣ - [Recursive Descent Parser](https://github.com/Pip-Install-Party/Recursive-Descent-Parser)  
4️⃣ - [Symbol Table](https://github.com/Pip-Install-Party/Symbol-Table)  
5️⃣ - [Abstract Syntax Tree](https://github.com/Pip-Install-Party/Abstract-Syntax-Tree)  
6️⃣ - [Interpreter](https://github.com/Pip-Install-Party/Interpreter) (Not Yet Functional)  

See the [open issues](https://github.com/github_username/repo_name/issues) for a full list of proposed features (and known issues).

<!-- USAGE EXAMPLES -->
## Usage

The interpreter is capable of producing output for code written in the C language. The program may also work with *some* programs written 
in C++, however differences in reserved words (such as int) may lead to unexpected output. Results with langages other than C are not guaranteed. 

See <a href="#readme-getting-started">getting-started</a>.


### Testing 

In addition to interpretting code, this program also lets you test its various components. 
Upon starting the program, the interface will display an option to "test components". 
Selecting this option will allow you to view the output of the following components:

1. Comment Removal
2. Tokenization
3. Recursive Descent Parser
4. Symbol Table
5. Abstract Syntax Tree


<!-- CONTRIBUTING -->
## Contributors

Five students worked together to construct the interpreter. 

<a href="https://github.com/Pip-Install-Party/Interpreter/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=Pip-Install-Party/Interpreter" alt="contrib.rocks image" />
</a>


<!-- CONTACT -->
### Connect With Us

Blake Marshall - [LinkedIn](linkedin.com/in/blakemarshalll)  
Brandon Robinson - [LinkedIn](linkedin.com/in/brandon-robinson-uscg)  
Holden Ea - [LinkedIn](linkedin.com/in/holden-ea-28a535208)  
Jacob Sellers - [LinkedIn](linkedin.com/in/jacob-sellers-83840826a)  
Rolando Yax - [LinkedIn](linkedin.com/in/rolandoyax)  

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/Pip-Install-Party/Interpreter.svg?style=for-the-badge
[contributors-url]: https://github.com/Pip-Install-Party/Interpreter/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/Pip-Install-Party/Interpreter.svg?style=for-the-badge
[forks-url]: https://github.com/Pip-Install-Party/Interpreter/network/members
[stars-shield]: https://img.shields.io/github/stars/Pip-Install-Party/Interpreter.svg?style=for-the-badge
[stars-url]: https://github.com/Pip-Install-Party/Interpreter/stargazers
[issues-shield]: https://img.shields.io/github/issues/Pip-Install-Party/Interpreter.svg?style=for-the-badge
[issues-url]: https://github.com/Pip-Install-Party/Interpreter/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge

[license-url]: https://github.com/Pip-Install-Party/Interpreter/blob/main/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
