package model.entity;

public class SudokuCell {

    private int value;
    private boolean isEditable;
    private CellStatus status;

    public enum CellStatus {
        NO_GUESS,
        WRONG_GUESS,
        RIGHT_GUESS
    }

    public SudokuCell(int value) {
        this.value = value;
    }

    public boolean isEditable() {
        return isEditable;
    }

    public void setEditable() {
        this.value = 0;
        this.isEditable = true;
        this.status = CellStatus.NO_GUESS;
    }

    public int getValue() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
    }

    public void setStatus(CellStatus status) {
        this.status = status;
    }

    public boolean isCorrect() {
        return status == CellStatus.RIGHT_GUESS;
    }
}
