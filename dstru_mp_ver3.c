#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

// Define constants for the sets
#define MAX_A 4 // A = {1, 2, 3, 4}
#define MAX_P (MAX_A * MAX_A) // P = A × A (16 pairs)

// Structure to represent a position (pair)
typedef struct {
    int x;
    int y;
} Position;

// Structure to represent sets of positions
typedef struct {
    Position elements[MAX_P];
    int count;
} PositionSet;

// Global variables (system variables)
PositionSet Uno, Dos, Tres, F;
bool turn, go, over;

// Function prototypes
void initializeSets();
bool isInSet(Position pos, PositionSet set);
void addToSet(Position pos, PositionSet *set);
void removeFromSet(Position pos, PositionSet *set);
void updateF();
bool isInW(PositionSet set);
bool checkGameOver();
void nextPlayerMove(Position pos);
void printGameState();
void printSet(PositionSet set, const char *setName);
void gameLoop();
bool isInC(PositionSet set);
bool isInT(PositionSet set);
void transferToDos(Position pos);
void validateGameState();
bool getValidCoordinates(int *x, int *y);

// Initialize the system
void initializeSets() {
    Uno.count = 0;
    Dos.count = 0;
    Tres.count = 0;
    
    F.count = 0;
    for (int i = 1; i <= MAX_A; i++) {
        for (int j = 1; j <= MAX_A; j++) {
            Position pos = {i, j};
            addToSet(pos, &F);
        }
    }
    
    turn = true;
    go = false;
    over = false;
}

// Check if a position is in a set
bool isInSet(Position pos, PositionSet set) {
    for (int i = 0; i < set.count; i++) {
        if (set.elements[i].x == pos.x && set.elements[i].y == pos.y) {
            return true;
        }
    }
    return false;
}

// Add a position to a set
void addToSet(Position pos, PositionSet *set) {
    if (!isInSet(pos, *set) && set->count < MAX_P) {
        set->elements[set->count] = pos;
        set->count++;
    }
}

// Remove a position from a set
void removeFromSet(Position pos, PositionSet *set) {
    for (int i = 0; i < set->count; i++) {
        if (set->elements[i].x == pos.x && set->elements[i].y == pos.y) {
            for (int j = i; j < set->count - 1; j++) {
                set->elements[j] = set->elements[j + 1];
            }
            set->count--;
            break;
        }
    }
}

// Transfer position from Uno or Tres to Dos
void transferToDos(Position pos) {
    if (isInSet(pos, Uno)) {
        removeFromSet(pos, &Uno);
        addToSet(pos, &Dos);
    } else if (isInSet(pos, Tres)) {
        removeFromSet(pos, &Tres);
        addToSet(pos, &Dos);
    }
}

// Update F = P - (Uno ∪ Tres ∪ Dos)
void updateF() {
    F.count = 0;
    for (int i = 1; i <= MAX_A; i++) {
        for (int j = 1; j <= MAX_A; j++) {
            Position pos = {i, j};
            if (!isInSet(pos, Uno) && !isInSet(pos, Tres) && !isInSet(pos, Dos)) {
                addToSet(pos, &F);
            }
        }
    }
}

// Check if a set is a subset of W (W = C - T)
bool isInW(PositionSet set) {
    return isInC(set) && !isInT(set);
}

// Check if set is in C
bool isInC(PositionSet set) {
    Position C1[4] = {{1,1}, {1,2}, {1,3}, {1,4}};
    Position C2[4] = {{1,1}, {2,2}, {3,3}, {4,4}};
    Position C3[4] = {{1,4}, {2,3}, {3,2}, {4,1}};
    Position C4[4] = {{4,1}, {4,2}, {4,3}, {4,4}};
    
    if (set.count != 4) return false;
    
    bool matchesC1 = true, matchesC2 = true, matchesC3 = true, matchesC4 = true;
    for (int i = 0; i < 4; i++) {
        bool found1 = false, found2 = false, found3 = false, found4 = false;
        for (int j = 0; j < set.count; j++) {
            if (C1[i].x == set.elements[j].x && C1[i].y == set.elements[j].y) found1 = true;
            if (C2[i].x == set.elements[j].x && C2[i].y == set.elements[j].y) found2 = true;
            if (C3[i].x == set.elements[j].x && C3[i].y == set.elements[j].y) found3 = true;
            if (C4[i].x == set.elements[j].x && C4[i].y == set.elements[j].y) found4 = true;
        }
        if (!found1) matchesC1 = false;
        if (!found2) matchesC2 = false;
        if (!found3) matchesC3 = false;
        if (!found4) matchesC4 = false;
    }
    
    return matchesC1 || matchesC2 || matchesC3 || matchesC4;
}

// Check if a set is in T
// T is the equality relation: {(1,1), (2,2), (3,3), (4,4)}
bool isInT(PositionSet set) {
    // First, ensure we have exactly the 4 positions on the diagonal
    if (set.count != 4) return false;
    
    // Check for all diagonal positions
    bool has1 = false, has2 = false, has3 = false, has4 = false;
    for (int i = 0; i < set.count; i++) {
        Position pos = set.elements[i];
        // Only accept positions where x equals y (diagonal)
        if (pos.x != pos.y) return false;
        
        if (pos.x == 1 && pos.y == 1) has1 = true;
        if (pos.x == 2 && pos.y == 2) has2 = true;
        if (pos.x == 3 && pos.y == 3) has3 = true;
        if (pos.x == 4 && pos.y == 4) has4 = true;
    }
    
    // Must have all diagonal positions
    return has1 && has2 && has3 && has4;
}

// Validate the game state for consistency
void validateGameState() {
    // Check for overlaps between sets
    for (int i = 1; i <= MAX_A; i++) {
        for (int j = 1; j <= MAX_A; j++) {
            Position pos = {i, j};
            int count = 0;
            
            if (isInSet(pos, Uno)) count++;
            if (isInSet(pos, Dos)) count++;
            if (isInSet(pos, Tres)) count++;
            
            if (count > 1) {
                printf("ERROR: Position (%d,%d) appears in multiple sets!\n", pos.x, pos.y);
                
                // Fix by prioritizing Dos, then Uno, then Tres
                if (isInSet(pos, Dos)) {
                    if (isInSet(pos, Uno)) removeFromSet(pos, &Uno);
                    if (isInSet(pos, Tres)) removeFromSet(pos, &Tres);
                } else if (isInSet(pos, Uno)) {
                    if (isInSet(pos, Tres)) removeFromSet(pos, &Tres);
                }
            }
            
            // Ensure F is properly maintained
            if (isInSet(pos, F) && (isInSet(pos, Uno) || isInSet(pos, Dos) || isInSet(pos, Tres))) {
                printf("ERROR: Position (%d,%d) in F but also in other sets!\n", pos.x, pos.y);
                removeFromSet(pos, &F);
            }
            
            if (!isInSet(pos, F) && !isInSet(pos, Uno) && !isInSet(pos, Dos) && !isInSet(pos, Tres)) {
                printf("ERROR: Position (%d,%d) not in any set!\n", pos.x, pos.y);
                addToSet(pos, &F);
            }
        }
    }
    
    // Verify F is correctly calculated
    PositionSet expectedF;
    expectedF.count = 0;
    for (int i = 1; i <= MAX_A; i++) {
        for (int j = 1; j <= MAX_A; j++) {
            Position pos = {i, j};
            if (!isInSet(pos, Uno) && !isInSet(pos, Tres) && !isInSet(pos, Dos)) {
                addToSet(pos, &expectedF);
            }
        }
    }
    
    // Check if F matches expected F
    bool fCorrect = true;
    if (F.count != expectedF.count) {
        fCorrect = false;
    } else {
        for (int i = 0; i < F.count; i++) {
            if (!isInSet(F.elements[i], expectedF)) {
                fCorrect = false;
                break;
            }
        }
    }
    
    if (!fCorrect) {
        printf("ERROR: F is not correctly calculated! Fixing...\n");
        updateF();
    }
}

// Check if game is over
bool checkGameOver() {
    bool unoInW = isInW(Uno);
    bool tresInW = isInW(Tres);
    bool fEmpty = (F.count == 0);
    over = unoInW || tresInW || fEmpty;
    return over;
}

// Handle a player's move
void nextPlayerMove(Position pos) {
    if (over) {
        printf("Game is already over!\n");
        return;
    }
    
    // First, validate current game state
    validateGameState();
    
    if (turn && go && isInSet(pos, F)) {
        // Player 1 adds to Uno
        addToSet(pos, &Uno);
        removeFromSet(pos, &F);  // Explicitly remove from F
        turn = !turn;
        go = !go;
        printf("Player 1 added position (%d,%d) to Uno\n", pos.x, pos.y);
    } else if (!turn && (isInSet(pos, Uno) || isInSet(pos, Tres))) {
        // Player 2 transfers to Dos
        transferToDos(pos);
        turn = !turn;
        printf("Player 2 transferred position (%d,%d) to Dos\n", pos.x, pos.y);
    } else if (turn && !go && isInSet(pos, F)) {
        // Player 1 adds to Tres
        addToSet(pos, &Tres);
        removeFromSet(pos, &F);  // Explicitly remove from F
        go = !go;
        printf("Player 1 added position (%d,%d) to Tres\n", pos.x, pos.y);
    } else {
        if (isInSet(pos, Dos)) {
            printf("Invalid move! Position (%d,%d) is already in Dos.\n", pos.x, pos.y);
        } else if (turn && go) {
            printf("Invalid move! Player 1 must select a free position from F to add to Uno.\n");
        } else if (!turn) {
            printf("Invalid move! Player 2 must select a position from Uno or Tres to transfer to Dos.\n");
        } else if (turn && !go) {
            printf("Invalid move! Player 1 must select a free position from F to add to Tres.\n");
        }
        return;
    }
    
    // Update F after each move and validate again
    updateF();
    validateGameState();
    
    checkGameOver();
}

// Get valid coordinates from user input (handles non-numeric input)
bool getValidCoordinates(int *x, int *y) {
    char input[100];
    char xChar, yChar;
    int result;
    
    // Read the entire line as a string
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return false;
    }
    
    // Try to parse two integers separated by space
    result = sscanf(input, "%d %d", x, y);
    
    // If we didn't successfully parse two integers
    if (result != 2) {
        // Check if non-numeric input was given
        result = sscanf(input, "%c %c", &xChar, &yChar);
        if (result > 0) {
            if (!isdigit(xChar) || (result > 1 && !isdigit(yChar))) {
                printf("Invalid input! Please enter numeric coordinates.\n");
                return false;
            }
        }
        printf("Invalid input format! Please enter two numbers separated by space.\n");
        return false;
    }
    
    // Check if the coordinates are within range
    if (*x < 1 || *x > MAX_A || *y < 1 || *y > MAX_A) {
        printf("Invalid position! Coordinates must be between 1 and %d.\n", MAX_A);
        return false;
    }
    
    return true;
}

// Print the current state of the game
void printGameState() {
    printf("\n--- Game State ---\n");
    printf("Grid:\n");
    printf("    1 2 3 4\n");
    for (int i = 1; i <= MAX_A; i++) {
        printf("%d   ", i);
        for (int j = 1; j <= MAX_A; j++) {
            Position pos = {i, j};
            char symbol = '.';
            // A position should only be in one set at a time
            if (isInSet(pos, Uno)) symbol = 'U';
            else if (isInSet(pos, Tres)) symbol = 'T';
            else if (isInSet(pos, Dos)) symbol = 'D';
            
            printf("%c ", symbol);
        }
        printf("\n");
    }
    
    printSet(Uno, "Uno");
    printSet(Dos, "Dos");
    printSet(Tres, "Tres");
    printSet(F, "F");
    
    printf("turn = %s\n", turn ? "true" : "false");
    printf("go = %s\n", go ? "true" : "false");
    printf("over = %s\n", over ? "true" : "false");
    
    if (!over) {
        if (turn && go) {
            printf("Player 1's turn to add to Uno\n");
        } else if (!turn) {
            printf("Player 2's turn to transfer to Dos\n");
        } else if (turn && !go) {
            printf("Player 1's turn to add to Tres\n");
        }
    }
}

// Print a set
void printSet(PositionSet set, const char *setName) {
    printf("%s = {", setName);
    for (int i = 0; i < set.count; i++) {
        printf("(%d,%d)", set.elements[i].x, set.elements[i].y);
        if (i < set.count - 1) printf(", ");
    }
    printf("}\n");
}

// Main game loop
void gameLoop() {
    int x, y;
    Position pos;
    
    while (!over) {
        printGameState();
        
        printf("Enter position (x y): ");
        
        // Get valid coordinates, retry if input is invalid
        if (!getValidCoordinates(&x, &y)) {
            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        pos.x = x;
        pos.y = y;
        nextPlayerMove(pos);
    }
    
    printGameState();
    
    // Check win conditions in priority order
    if (isInW(Uno)) {
        printf("Player 1 Wins with Uno!\n");
    } else if (isInW(Tres)) {
        printf("Player 1 Wins with Tres!\n");
    } else if (F.count == 0) {
        printf("Player 2 Wins with Dos!\n");
    }
}

int main() {
    initializeSets();
    printf("Welcome to the CCDSTRU Game!\n");
    printf("Player 1 controls Uno and Tres, Player 2 controls Dos\n");
    printf("Game objective: Player 1 wins if Uno or Tres forms one of the winning patterns.\n");
    printf("Player 2 wins if they can prevent Player 1 from forming a winning pattern.\n");
    gameLoop();
    return 0;
}