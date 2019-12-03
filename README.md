# Thermal Transference Visualizer

### General description
This program provides a visualization of the thermal transfer on a flat surface. It is required that the user enters a file in [.csv](https://en.wikipedia.org/wiki/Comma-separated_values) format whose cells represent different temperatures that will be placed in a matrix and allow the program to simulate the heat exchange between them. If the dimension of the ``.csv`` file is excessively big, the final MainWindow will resize itself to an optimum size so it doesn't exceeds the visible range. As an additional detail, the edges will have the same temperature and this will remain unchanged throughout the simulation, representing the constant exposure to the heat of the surface. 

The simulation consists in updating the values of temperature matrix  by calculating, for each cell, the average of its neighboring temperatures (right, left, up, and down) until the equilibrium state is reached. It is said that the surface reaches a state of thermal equilibrium when the temperature of all its cells, from one instant to another, does not change more than a very small amount. This maximum level of variation will be called epsilon and will also be defined by the user, as well as the temperature unit to use, which could be either Celsius or Fahrenheit.

The behavior of the temperature is represented by a matrix of pixels, whose colors will follow the [RGB](https://en.m.wikipedia.org/wiki/RGB_color_model) model, and will mutate continuously until the state of thermal equilibrium is reached. The refresh interval of the interface is customizable and also the simulation time will be reported. Simbology to use in the visualization:

* **Blue:** Coldest temperature.
* **Cyan:** Cold temperature.
* **Green:**  Warm temperature.
* **Yellow:** Hot.
* **Red:** Hottest temperature.


<center>
    <img src="https://i.imgur.com/1WBGYQG.png" width="450" height="400" alt="heat map">
</center>


### How to use it

The main window will have an ``Open File`` button which the user needs to click in order to choose the ``.csv`` file from the computer's file explorer. After that, the user will see two different input text lines, the first one is for the epsilon to use and the second one is for the time (miliseconds) in which the program will update its interface. 


### Resources

* [QTProject](https://www.qt.io).

* [RGB Color Model](https://www.rapidtables.com/web/color/RGB_Color.html).

* [C++](http://www.cplusplus.com/).

### About this project

This is an assigment of the Parallel and Concurrent Programing course at the University of Costa Rica, under the guidance of Jeisson Hidalgo Céspedes, and is developed by:

* Gloriana Mora Villalta. Email: gloriana.moravillalta@ucr.ac.cr.

* Iván Chavarría Vega. Email: ivan.chavarriavega@ucr.ac.cr.

This project is protected under the [GNU](https://www.gnu.org/licenses/gpl-3.0.en.html) General Public License.
