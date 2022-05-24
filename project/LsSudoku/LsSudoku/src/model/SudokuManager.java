package model;

import model.entity.Sudoku;
import model.entity.SudokuCell;
import model.entity.Sudoku.SudokuMode;

import java.util.List;

public class SudokuManager {

    private final Sudoku sudoku;

    public SudokuManager() {
        this.sudoku = new Sudoku();
    }

    public SudokuCell[][] getUnsolvedSudoku(SudokuMode mode) {
        sudoku.generateSudoku(mode);
        return sudoku.getUnsolvedSudoku();
    }

    public boolean validateSudoku() {
        return sudoku.isSolutionValid();
    }

    public int getSudokuScore() {
        return sudoku.getScore();
    }

    public List<int[]> getSudokuErrors() {
        return sudoku.getErrors();
    }

    public int[] moveCurrentCell(int[] currentCell) {
        return sudoku.moveCurrentCell(currentCell);
    }
    public int[] moveCurrentCell(char direction) {
        return sudoku.moveCurrentCell(direction);
    }

    public int[] getCurrentCell() {
        return sudoku.getCurrentCell();
    }

    public void setCurrentCellValue(int num) {
        sudoku.setCurrentCellValue(num);
    }
}
