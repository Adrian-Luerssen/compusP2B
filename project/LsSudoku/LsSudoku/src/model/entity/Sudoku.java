package model.entity;

import model.entity.SudokuCell.CellStatus;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Sudoku {

    public static final int NUM_CELLS = 9;
    private static final int SQRT_NUM_CELLS = (int) Math.sqrt(NUM_CELLS);

    private final SudokuCell[][] solvedSudoku;
    private final SudokuCell[][] unsolvedSudoku;

    private int unsolvedCells;
    private int[] currentCell;

    public enum SudokuMode {
        SUPER_EASY,
        EASY,
        MEDIUM,
        HARD,
        GOD
    }

    public Sudoku() {
        this.solvedSudoku = new SudokuCell[NUM_CELLS][NUM_CELLS];
        this.unsolvedSudoku = new SudokuCell[NUM_CELLS][NUM_CELLS];
    }

    public void setMode(SudokuMode mode) {
        switch (mode) {
            case SUPER_EASY -> unsolvedCells = 10;
            case EASY -> unsolvedCells = 40;
            case MEDIUM -> unsolvedCells = 48;
            case HARD -> unsolvedCells = 56;
            case GOD -> unsolvedCells = 64;
        }
    }
    public SudokuCell[][] getUnsolvedSudoku() {
        return unsolvedSudoku;
    }

    /* -------------------------------------- SUDOKU CURRENT CELL -------------------------------------- */

    private void generateCurrentCell() {
        for (int i = 0; i < NUM_CELLS; i++) {
            for (int j = 0; j < NUM_CELLS; j++) {
                if (unsolvedSudoku[i][j].isEditable()) {
                    currentCell = new int[]{i, j};
                    return;
                }
            }
        }
    }

    public int[] moveCurrentCell(int[] currentCell) {
        if (unsolvedSudoku[currentCell[0]][currentCell[1]].isEditable()) {
            this.currentCell = currentCell;
        }

        return this.currentCell;
    }

    public int[] moveCurrentCell(char direction) {
        switch (direction) {
            case 'w' -> moveUp(currentCell[0], currentCell[1]);
            case 's' -> moveDown(currentCell[0], currentCell[1]);
            case 'a' -> moveLeft(currentCell[0], currentCell[1]);
            case 'd' -> moveRight(currentCell[0], currentCell[1]);
        }
        return currentCell;
    }

    /* Top and left movements */
    private void moveUp(int i, int j) {
        do {
            i--;
            if (i < 0) {
                i = NUM_CELLS - 1;
                j--;
                if (j < 0) {
                    j = NUM_CELLS - 1;
                }
            }
        } while (!unsolvedSudoku[i][j].isEditable());

        currentCell = new int[]{i, j};
    }

    private void moveLeft(int i, int j) {
        do {
            j--;
            if (j < 0) {
                j = NUM_CELLS - 1;
                i--;
                if (i < 0) {
                    i = NUM_CELLS - 1;
                }
            }
        } while (!unsolvedSudoku[i][j].isEditable());

        currentCell = new int[]{i, j};
    }

    /* Down and right movements */
    private void moveDown(int i, int j) {
       do {
           i++;
           if (i > NUM_CELLS - 1) {
               i = 0;
               j++;
               if (j > NUM_CELLS - 1) {
                   j = 0;
               }
           }
       } while (!unsolvedSudoku[i][j].isEditable());

        currentCell = new int[]{i, j};
    }

    private void moveRight(int i, int j) {
        do {
            j++;
            if (j > NUM_CELLS - 1) {
                j = 0;
                i++;
                if (i > NUM_CELLS - 1) {
                    i = 0;
                }
            }
        } while (!unsolvedSudoku[i][j].isEditable());

        currentCell = new int[]{i, j};
    }

    public int[] getCurrentCell() {
        return currentCell;
    }

    public void setCurrentCellValue(int num) {
        int i = currentCell[0];
        int j = currentCell[1];
        boolean isGuessCorrect = num == solvedSudoku[i][j].getValue();

        unsolvedSudoku[i][j].setValue(num);
        unsolvedSudoku[i][j].setStatus((isGuessCorrect) ? CellStatus.RIGHT_GUESS : CellStatus.WRONG_GUESS);
    }

    /* -------------------------------------- SUDOKU VALIDATIONS -------------------------------------- */

    public boolean isSolutionValid() {
        for (int i = 0; i < NUM_CELLS; i++) {
            for (int j = 0; j < NUM_CELLS; j++) {
                if (unsolvedSudoku[i][j].isEditable() && !unsolvedSudoku[i][j].isCorrect())
                    return false;
            }
        }

        return true;
    }

    public int getScore() {
        int score = 0;
        for (int i = 0; i < NUM_CELLS; i++) {
            for (int j = 0; j < NUM_CELLS; j++) {
                if (unsolvedSudoku[i][j].isEditable() && unsolvedSudoku[i][j].isCorrect())
                    score++;
            }
        }

        return score;
    }

    public List<int[]> getErrors() {
        List<int[]> errorList = new ArrayList<>();

        for (int i = 0; i < NUM_CELLS; i++) {
            for (int j = 0; j <NUM_CELLS; j++) {
                if (unsolvedSudoku[i][j].isEditable() && !unsolvedSudoku[i][j].isCorrect()) {
                    errorList.add(new int[]{i, j});
                }
            }
        }

        return errorList;
    }

    /* -------------------------------------- SUDOKU GENERATOR -------------------------------------- */

    public void generateSudoku(SudokuMode mode) {
        setMode(mode);
        // Clear previous results
        for (int i = 0; i < NUM_CELLS; i++) {
            for (int j = 0; j < NUM_CELLS; j++) {
                solvedSudoku[i][j] = new SudokuCell(0);
            }
        }

        // Part I
        fillMainDiagonalMatrices();
        // Part II
        fillRemainingMatrices(0, SQRT_NUM_CELLS);
        // Part III
        removeCells();

        // Get first editable cell
        generateCurrentCell();

        printSudoku(solvedSudoku);
    }

    /* -------------------------------------- GENERATION VALIDATIONS -------------------------------------- */

    private boolean isCellValid(int i, int j, int num) {
        return !isNumUsedInRow(i, num) &&
               !isNumUsedInCol(j, num) &&
               !isNumUsedInMatrix(i-i% SQRT_NUM_CELLS, j-j% SQRT_NUM_CELLS, num);
    }

    private boolean isNumUsedInMatrix(int row, int col, int num)
    {
        for (int i = 0; i < SQRT_NUM_CELLS; i++) {
            for (int j = 0; j < SQRT_NUM_CELLS; j++) {
                if (solvedSudoku[row + i][col + j].getValue() == num) {
                    return true;
                }
            }
        }

        return false;
    }

    private boolean isNumUsedInRow(int row, int num) {
        for (int j = 0; j < NUM_CELLS; j++) {
            if (solvedSudoku[row][j].getValue() == num) {
                return true;
            }
        }

        return false;
    }

    private boolean isNumUsedInCol(int col, int num) {
        for (int i = 0; i < NUM_CELLS; i++) {
            if (solvedSudoku[i][col].getValue() == num) {
                return true;
            }
        }

        return false;
    }

    /* -------------------------------------- PART I -------------------------------------- */

    private void fillMainDiagonalMatrices() {

        //  X |   |
        // -----------
        //    | X |
        // -----------
        //    |   | X

        for (int i = 0; i < NUM_CELLS; i = i + SQRT_NUM_CELLS) {
            fillMatrix(i, i);
        }
    }

    private void fillMatrix(int row, int col) {
        int num;

        for (int i = 0; i < SQRT_NUM_CELLS; i++) {
            for (int j = 0; j < SQRT_NUM_CELLS; j++) {
                do {
                    num = randomNumber(NUM_CELLS);
                }
                while (isNumUsedInMatrix(row, col, num));

                solvedSudoku[row + i][col + j].setValue(num);
            }
        }
    }

    /* -------------------------------------- PART II -------------------------------------- */

    private boolean fillRemainingMatrices(int i, int j) {
        if (j >= NUM_CELLS && i < NUM_CELLS - 1){
            i = i + 1;
            j = 0;
        }
        if (i >= NUM_CELLS && j >= NUM_CELLS) {
            return true;
        }

        if (i < SQRT_NUM_CELLS) {
            if (j < SQRT_NUM_CELLS) {
                j = SQRT_NUM_CELLS;
            }
        } else if (i < NUM_CELLS - SQRT_NUM_CELLS) {
            if (j == (i / SQRT_NUM_CELLS) * SQRT_NUM_CELLS) {
                j = j + SQRT_NUM_CELLS;
            }
        }
        else {
            if (j == NUM_CELLS - SQRT_NUM_CELLS) {
                i = i + 1;
                j = 0;
                if (i >= NUM_CELLS) {
                    return true;
                }
            }
        }

        for (int num = 1; num <= NUM_CELLS; num++) {
            if (isCellValid(i, j, num)) {
                solvedSudoku[i][j].setValue(num);
                if (fillRemainingMatrices(i, j + 1)) {
                    return true;
                }

                solvedSudoku[i][j].setValue(0);
            }
        }
        return false;
    }

    /* -------------------------------------- PART III -------------------------------------- */

    private void removeCells() {
        /* Copy matrix */
        for (int i = 0; i < NUM_CELLS; i++) {
            for (int j = 0; j < NUM_CELLS; j++) {
                unsolvedSudoku[i][j] = new SudokuCell(solvedSudoku[i][j].getValue());
            }
        }

        int count = unsolvedCells;
        while (count != 0) {
            int cell = randomNumber(NUM_CELLS * NUM_CELLS) - 1;

            int i = (cell / NUM_CELLS);
            int j = cell % 9;


            if (unsolvedSudoku[i][j].getValue() != 0) {
                unsolvedSudoku[i][j].setEditable();
                count--;
            }
        }
    }

    /* -------------------------------------- UTILITIES -------------------------------------- */

    private int randomNumber(int num) {
        return new Random().nextInt(num) + 1;
    }

    public void printSudoku(SudokuCell[][] sudoku) {
        for (int i = 0; i < NUM_CELLS; i++) {
            for (int j = 0; j < NUM_CELLS; j++) {
                System.out.print(sudoku[i][j].getValue() + " ");
            }
            System.out.println();
        }
        System.out.println();
    }

}
