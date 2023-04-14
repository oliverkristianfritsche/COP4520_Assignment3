Run instructions(compiled on a linux machine)

Problem 1:  $g++ Assignment3_p1.cpp -pthread
	    $./a.out

Problem 2:  $g++ Assignment3_p2.cpp -pthread
	    $./a.out


Problem 1)

    The problem with the Minotaur's servants' strategy is that they were trying to add 
    and remove presents from the list simultaneously, which could have led to incorrect 
    behavior due to concurrency issues. To improve their strategy, I have designed a concurrent 
    linked list, where each operation (add_present,, remove_present, and contains) is protected 
    by a mutex to ensure that the operations are synchronized and avoid any race conditions. This 
    should ensure that the number of presents and "Thank you" notes is consistent.

    The general process remains the same though:

        First, add all the presents to the concurrent linked list.
        Next, have the servants remove the presents from the list one by one and write "Thank you" notes.
        Keep track of the number of "Thank you" notes written by each servant.
        Finally, verify that the total number of "Thank you" notes matches the number of presents.

    
		

	Statement of correctness:
		The program simulates inserting and removing items from a linked list,taking turns until all items have been inserted and deleted once. It uses an array of threads to represent each servant and a linked list to represent the chain of presents. The program uses a mutex to ensure that multiple threads do not contend over shared resources. Overall, the program is correct in that it simulates the problem as described.

	Statement of efficiency:
		The program uses threads to simulate the servants, which can be an efficient way to handle multiple concurrent tasks. The program also uses a mutex to prevent race conditions when multiple threads access shared data. 	
	Experiemntal evaluation:
		The program was successful since it only terminates once the task in done therefor all threads had entered the maze atleast once.
        As well as checking the ordering of the linked list for smaller values of n(num presents) to ensure the algorithm was working properly
	
Problem 2)

	The Mars Rover temperature sensor problem was addressed using a `TemperatureRecorder` 
    class that stores temperature readings from 8 threads simulating the sensors. 
    The program uses a mutex and a condition variable to ensure correct synchronization 
    between the sensor threads and the report generator thread, preventing any race conditions
    and ensuring that the report is generated only after all temperature readings have been taken.

    The general process is as follows:

        Each sensor thread generates temperature readings and records them in the shared TemperatureRecorder object.
        The report generator thread waits until all readings have been taken.
        The report generator calculates the top 5 highest and lowest temperatures, and the 10-minute interval with the largest temperature difference.
        The report is then printed to the console.

    Statement of correctness:
        The program correctly simulates the operation of 8 temperature sensors and the generation of an hourly report using threads, a mutex, and a condition variable for synchronization. The temperature readings are taken every simulated minute, and the report is generated once all readings have been taken. The program correctly calculates the top 5 highest and lowest temperatures, as well as the 10-minute interval with the largest temperature difference.

    Statement of efficiency:
        The program has a linear time complexity with respect to the number of temperature readings. The `std::nth_element` function, which has linear time complexity on average, is used to find the top 5 highest and lowest temperatures. The program uses threads to simulate the sensors, which can be an efficient way to handle multiple concurrent tasks. The use of a mutex and condition variable ensures correct synchronization between the threads.

    Experimental evaluation:
        The program was successful in simulating the operation of the temperature sensors and generating the report as described. The program terminates once the report has been generated, and the report contains the correct information regarding the top 5 highest and lowest temperatures, and the 10-minute interval with the largest temperature difference. The program demonstrates the correct use of threads, mutex, and condition variables for managing concurrency in a multi-threaded environment.
