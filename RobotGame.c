#include <ncurses/ncurses.h>
#include <stdlib.h>
#include <time.h>

#define ROW 25
#define COL 100
#define DELAY 50 // delay in milliseconds (speed of robot)

// position of robot
int robot[2];
int direction = KEY_RIGHT;

// number of lives
int lives = 3;

// score
int score = 0;

// person to be rescued
int person[2];
 
// danger locations' position and danger count
int danger_count = 2;
int** danger;

// level and the rescue count
int level = 1;
int rescue_count = 0;

// Function to display the level
void display_level() {

    // Move the cursor
    move(25, 12);
    // Print the level
    printw("Level: %d", level);
}

// Function to initialize the robot
void init_robot() {
    // Place the robot in a random location within the rectangle
    srand(time(NULL));
    int row = rand() % (ROW-4) + 2;
    int col = rand() % (COL-4) + 2;
    robot[0] = row;
    robot[1] = col;
}

// Function to initialize the person
void init_person() {
    // Place the person in a random location within the rectangle
    srand(time(NULL));
    int row = rand() % (ROW-5) + 2;
    int col = rand() % (COL-5) + 2;
    person[0] = row;
    person[1] = col;
}

// Function to initialize the danger locations
void init_danger() {
    // Allocate memory for the danger locations
    danger = malloc(danger_count * sizeof(int*));
    for (int i = 0; i < danger_count; i++) {
        danger[i] = malloc(2 * sizeof(int));
    }
    // Place the danger locations in random locations within the rectangle
    srand(time(NULL));
    for (int i = 0; i < danger_count; i++) {
        int row = rand() % (ROW-5) + 2;
        int col = rand() % (COL-4) + 2;
        danger[i][0] = row;
        danger[i][1] = col;
    }
}

// Function to update the robot's position
void update_robot(int ch) {
    if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT) {
        direction = ch;
    }
    switch (direction) {
        case KEY_UP:
            if (robot[0] > 2) 
            robot[0]--;
            else 
            {
                robot[0] = ROW-4; // Wrap around the top wall
                lives--; // Lose one life
            }
            break;
        case KEY_DOWN:
            if (robot[0] < ROW-2) 
            robot[0]++;
            else 
            {
                robot[0] = 2; // Wrap around the bottom wall
                lives--; // Lose one life
            }
            break;
        case KEY_LEFT:
            if (robot[1] > 2) 
            robot[1]--;
            else 
            {
                robot[1] = COL-2; // Wrap around the left wall
                lives--; // Lose one life
            }
            break;
        case KEY_RIGHT:
            if (robot[1] < COL-4) 
            robot[1]++;
            else 
            {
                robot[1] = 0; // Wrap around the right wall
                lives--; // Lose one life
            }
            break;
    }
}

// Function to check if the robot has rescued the person
void check_rescue() {
    if (robot[0] == person[0] && robot[1] == person[1]) {
        // Increase the score by 10
        score += 10;
        // Increase the rescue count by 1
        rescue_count++;
        // Check if the rescue count has reached 5
        if (rescue_count % 5 == 0) {
            // Increase the level by 1
            level++;
            // Increase the speed
            timeout(DELAY / level);
        }
        // Increase the danger count by 2
        danger_count += 2;
        // Free the old danger locations
        for (int i = 0; i < danger_count-2; i++) {
            free(danger[i]);
        }
        free(danger);
        // Initialize a new person and new danger locations
        init_person();
        init_danger();
    }
}

// Function to check if the robot has hit a danger location
void check_danger() {
    for (int i = 0; i < danger_count; i++) {
        if (robot[0] == danger[i][0] && robot[1] == danger[i][1]) {
            // Lose one life
            lives--;
            // Move the robot to a random location
            init_robot();
            break;
        }
    }
}

// Function to draw a rectangle
void draw_rectangle() {
    // Draw the top border
    mvaddch(0, 0, ACS_ULCORNER);
    for (int i = 1; i < COL-1; i++) {
        mvaddch(0, i, ACS_HLINE);
    }
    mvaddch(0, COL-1, ACS_URCORNER);

    // Draw the side borders
    for (int i = 1; i < ROW-1; i++) {
        mvaddch(i, 0, ACS_VLINE);
        mvaddch(i, COL-1, ACS_VLINE);
    }

    // Draw the bottom border
    mvaddch(ROW-1, 0, ACS_LLCORNER);
    for (int i = 1; i < COL-1; i++) {
        mvaddch(ROW-1, i, ACS_HLINE);
    }
    mvaddch(ROW-1, COL-1, ACS_LRCORNER);
}

// Function to display the lives
void display_lives() {
    // Move the cursor
    move(25, 22);
    // Print the lives
    printw("Lives: %d", lives);
}

// Function to display the score
void display_score() {
    // Move the cursor
    move(25, 0);
    // Print the score
    printw("Score: %d", score);
}

// Main function
int main()
 { 
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    mvprintw(ROW / 2, (COL - 20) / 2, "Welcome to this game. Press any key to begin.");
    // Refresh the screen
    refresh();
            
    // Wait indefinitely for user input
    timeout(-1);  
    getch();

    // Initialize 
    init_robot();
    init_person();
    init_danger();
    
    // Set the timeout for getch
    timeout(DELAY);
    
    // Main game loop
    while (1) {
        // Clear the screen
        clear();
        
        // Draw the rectangle
        draw_rectangle();
        
        // Display the robot
        mvaddch(robot[0], robot[1], '|[]');
        switch (direction) {
            case KEY_UP:
                mvaddch(robot[0]-1, robot[1], ACS_UARROW);
                break;
            case KEY_DOWN:
                mvaddch(robot[0]+1, robot[1], ACS_DARROW);
                break;
            case KEY_LEFT:
                mvaddch(robot[0], robot[1]-1, ACS_LARROW);
                break;
            case KEY_RIGHT:
                mvaddch(robot[0], robot[1]+1, ACS_RARROW);
                break;
        }
           
        // Display the danger locations
        for (int i = 0; i < danger_count; i++) {
            mvaddch(danger[i][0], danger[i][1], ACS_NEQUAL);
        }
        // Display the person
        mvaddch(person[0], person[1], ACS_STERLING);
    
        move(25, COL-20);
        // Print the level
        printw("Press q to quit");
        // Display the lives
        display_lives();
        
        // Display the score
        display_score();

        // Display the level
        display_level();
        
        // Refresh the screen
        refresh();
        
        // Wait for user input
        int ch = getch();
        if (ch == 'q') {
            break;
        }
        
        // Update the robot's position
        update_robot(ch);
        
        // Check if the robot has rescued the person
        check_rescue();
        
        // Check if the robot has hit a danger location
        check_danger();
        
        // Check if the game is over
        if (lives == 0) {
            // Clear the screen
            clear();
            // Print the game over message
            mvprintw(ROW / 2, (COL - 20) / 2, "Game Over! Press any key to exit.");
            // Refresh the screen
            refresh();
            
            // Wait indefinitely for user input
            timeout(-1);  
            getch();
            break;
        }
    }
    
    // End ncurses
    endwin();
    
    return 0;
}
