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
    <img src="Images/logo.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">Interpreter</h3>

  <p align="center">
    An interpreter for code in Backus-Naur Form. 
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
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#running">Running The Program</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributors">Contributors</a></li>
    <li><a href="#connect-with-us">Connect</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This program was developed as the culminating project for CS460: Programming Languages at Sonoma State University during the Fall 2024 semester. Collaboratively created by a team of five students, this project presents an interpreter designed to process and execute code written in Backus-Naur Form (BNF), a C-like programming language. The interpreter interprets the input in BNF, analyzes its structure and logic, and executes the corresponding computations to generate the desired output. This project exemplifies advanced concepts in parsing, syntax analysis, data structures, and interpreter design.

The interpreter was constructed through a modular design, composed of several key components that work in sequence to process the input code. These components include:

1.	[Comment Removal](https://github.com/Pip-Install-Party/Ignoring-Comments): Eliminates non-executable comments to prepare the input for further processing.
   
2.	[Tokenization](https://github.com/Pip-Install-Party/Tokenization): Breaks the input into meaningful tokens, such as keywords, identifiers, and symbols, forming the building blocks of the program.
   
3.	[Recursive Descent Parsing](https://github.com/Pip-Install-Party/Recursive-Descent-Parser): Analyzes the tokenized input based on the defined BNF grammar, ensuring syntactical correctness while building a structural representation of the code. Generates a Concrete Syntax Tree (CST) representing program strucutre.
   
4.	[Symbol Table Generation](https://github.com/Pip-Install-Party/Symbol-Table): Creates a mapping of variables, functions, and other symbols to their corresponding definitions and values, enabling efficient lookup during execution.
   
5.	[Abstract Syntax Tree (AST) Generation](https://github.com/Pip-Install-Party/Abstract-Syntax-Tree): Constructs a tree representation of the program that encapsulates its logical structure and relationships, serving as the foundation for execution.

Each component builds on the outputs of the previous stage, concluding in a fully functional interpreter capable of processing complex inputs and producing correct results. This layered approach highlights the importance of modularity and abstraction in software engineering and programming language design.

This repository contains additional functionality to further integrate these compoents and enable the interpreter's functionality. 

<!-- GETTING STARTED -->
<a id="readme-getting-started"></a>
## Getting Started

The interpreter is compatible with Windows, MacOS, and Linux. 

### Dependencies

This project utlizes ncurses when run on MacOS / Linux. This library can be insalled via:

#### MacOS 
```sh
brew install ncurses
```

#### Linux 

```sh
sudo apt-get install libncurses-dev
```


### Running The Program

#### MacOS / Linux 
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

#### Windows 
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

### File Output 

Output will be written to ```Output.txt``` in the working directory.
<!-- ROADMAP -->
## Roadmap

This project was implemented in stages. Each stage builds from the last and ultimately constructs an interpreter. 

1️⃣ - [Comment Removal](https://github.com/Pip-Install-Party/Ignoring-Comments)  
2️⃣ - [Tokenization](https://github.com/Pip-Install-Party/Tokenization)  
3️⃣ - [Recursive Descent Parser](https://github.com/Pip-Install-Party/Recursive-Descent-Parser)  
4️⃣ - [Symbol Table](https://github.com/Pip-Install-Party/Symbol-Table)  
5️⃣ - [Abstract Syntax Tree](https://github.com/Pip-Install-Party/Abstract-Syntax-Tree)  
6️⃣ - [Interpreter](https://github.com/Pip-Install-Party/Interpreter)

See the [open issues](https://github.com/Pip-Install-Party/Interpreter/issues) for a full list of proposed features (and known issues).

<!-- USAGE EXAMPLES -->
## Usage

The interpreter is capable of producing output for code written in Backus-Naur Form. The program may also work with *some* programs written 
in C, however differences in reserved words (such as string) may lead to unexpected output. 

See <a href="#readme-getting-started">getting-started</a>.


### Testing 

This program contains a series of files that can be used to test the interpreter. The program's interface will prompt you to make a selection from the available test files. 

In addition to interpretting code, this program also lets you test its various components. 
Upon starting the program, the interface will display a series of components to select from.
The following components can be selected:

1. Comment Removal
2. Tokenization
3. Recursive Descent Parser
4. Symbol Table
5. Abstract Syntax Tree
6. Interpreter

The results from the selected component will print to `Output.txt`.


<!-- CONTRIBUTORS -->
## Contributors

Five students worked together to construct the interpreter. 

<a href="https://github.com/Pip-Install-Party/Interpreter/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=Pip-Install-Party/Interpreter" alt="contrib.rocks image" />
</a>


<!-- CONNECT -->
### Connect With Us

Blake Marshall - [LinkedIn](https://www.linkedin.com/in/blakemarshalll)  
Brandon Robinson - [LinkedIn](https://www.linkedin.com/in/brandon-robinson-uscg)  
Holden Ea - [LinkedIn](https://www.linkedin.com/in/holden-ea-28a535208)  
Jacob Sellers - [LinkedIn](https://www.linkedin.com/in/jacob-sellers-83840826a)  
Rolando Yax - [LinkedIn](https://www.linkedin.com/in/rolandoyax)  

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.` for more information.

<!-- LICENSE -->
## Acknowledgments

Test files were provided by Robert Bruce.

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

[license-url]: https://github.com/Pip-Install-Party/Interpreter/blob/main/LICENSE.
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
