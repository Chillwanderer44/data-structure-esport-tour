/**
 * APUEC Integrated Tournament Management System
 * Main Program Entry Point
 * 
 * Course: CT077-3-2-DSTR (Data Structures)
 * Assignment: Lab Evaluation Work #2
 * 
 * Team Members & Tasks:
 * - Task 1: Match Scheduling & Player Progression [Member 1]
 * - Task 2: Tournament Registration & Player Queueing [Member 2] 
 * - Task 3: Live Stream & Spectator Queue Management [Your Name]
 * - Task 4: Game Result Logging & Performance History [Member 4]
 *
 * Data Structures Used: Stack, Queue, Priority Queue, Circular Queue
 */

 #include "apuec_integrated_system.hpp"
 #include <iostream>
 using namespace std;
 
 int main() {
     try {
         // Initialize the integrated tournament management system
         APUECIntegratedSystem tournamentSystem;
         
         // Run the main system interface
         tournamentSystem.runSystem();
         
     } catch (const exception& e) {
         cout << "System Error: " << e.what() << endl;
         cout << "Please restart the system and try again." << endl;
         return 1;
     }
     
     return 0;
 }