package controller;

import model.SudokuManager;
import model.TimeManager;
import model.TimeManager.TimeListener;
import model.entity.Serial;
import model.entity.Sudoku.SudokuMode;
import view.SudokuView;

import javax.swing.*;
import java.awt.event.*;
import java.util.List;

public class SudokuController implements ActionListener, MouseListener, KeyListener, TimeListener {

    private final SudokuView sudokuView;
    private final SudokuManager sudokuManager;
    private final TimeManager timeManager;

    private boolean isListenerEnabled;
    private String username;

    public SudokuController(SudokuView sudokuView, SudokuManager sudokuManager, TimeManager timeManager) {
        this.sudokuView = sudokuView;
        this.sudokuManager = sudokuManager;
        this.timeManager = timeManager;
        this.isListenerEnabled = false;
        this.username = "";
    }

    /* -------------------------------------- MODEL-VIEW -------------------------------------- */

    @Override
    public void updateTime(String time) {
        sudokuView.updateTime(time);
    }

    /* -------------------------------------- VIEW-MODEL -------------------------------------- */

    /* ActionListener */

    @Override
    public void actionPerformed(ActionEvent e) {
        switch (e.getActionCommand()) {
            case SudokuView.AC_GAME -> startNewGame();
            case SudokuView.AC_SERIAL -> sudokuView.updateSerialStatus(sudokuView.displaySerialDialog());
            case SudokuView.AC_SUBMIT -> stopSudoku();
        }
    }

    /* KeyListener */

    @Override
    public void keyTyped(KeyEvent e) {
        if (isListenerEnabled) {
            char keyPressedChar = e.getKeyChar();

            /* Clear cell */
            if (keyPressedChar == '\b') {
                sudokuView.setCurrentCellValue(0);
                sudokuManager.setCurrentCellValue(0);
                return;
            }

            processMove(keyPressedChar);
        }
    }

    /* MouseListener */

    @Override
    public void mouseClicked(MouseEvent e) {
        if (isListenerEnabled) {
            JPanel cellPanel = ((JPanel) e.getSource());
            sudokuView.setCurrentCell(sudokuManager.moveCurrentCell(sudokuView.getCurrentCell(cellPanel)));
        }
    }

    /* -------------------------------------- INTERNAL METHODS -------------------------------------- */

    private void startNewGame() {
        /* Dialog for new game */
        SudokuMode mode = sudokuView.displayNewGameDialog();

        if (mode != null) {
            /* Send ACK */
            sendAcknowledgement();

            /* Enable UI */
            isListenerEnabled = true;
            sudokuView.enableSubmitBtn(true);
            sudokuView.enableNewGameBtn(false);

            /* Set timer and username */
            sudokuView.setUsername(username);
            timeManager.startTimer();

            /* Display new generated sudoku */
            sudokuView.paintSudoku(sudokuManager.getUnsolvedSudoku(mode));
            sudokuView.setCurrentCell(sudokuManager.getCurrentCell());
        }
    }

    private void stopSudoku() {
        List<int[]> errors = sudokuManager.getSudokuErrors();
        int score = sudokuManager.getSudokuScore();

        /* Stop timer and reset username */
        timeManager.stopTimer();
        username = "";

        /* Disable UI */
        isListenerEnabled = false;
        sudokuView.enableSubmitBtn(false);
        sudokuView.enableNewGameBtn(true);

        /* Send score */
        sendScore(score);

        if (sudokuManager.validateSudoku()) {
            sudokuView.displayCorrectDialog("Sudoku Correction", "The Sudoku is correct! Your score is " + score);
        } else {
            sudokuView.setErrorCell(errors);
            /* Send errors */
            sendErrors(errors);
            sudokuView.displayErrorDialog("Sudoku Correction", "The Sudoku is NOT correct! Your score is " + score);
        }
    }

    private void processMove(char move) {
        int moveValue = Character.getNumericValue(move);

        if (moveValue >= 1 && moveValue <= 9) {
            sudokuView.setCurrentCellValue(moveValue);
            sudokuManager.setCurrentCellValue(moveValue);
        } else {
            move = Character.toLowerCase(move);

            if (move == 'w' || move == 's' || move == 'a' || move == 'd') {
                sudokuView.setCurrentCell(sudokuManager.moveCurrentCell(move));
            }
        }
    }

    /* -------------------------------------- SERIAL -------------------------------------- */

    public void getSerialInput() {
        Serial serial = Serial.getInstance();

        /* Check serial connectivity */
        if (serial.isConnected()) {
            /* Check for available bytes */
            if (serial.isBufferEmpty()) {
                return;
            }

            /* Read byte */
            char readCharacter = serial.readCharacter();

            /* Before game start */
            if (!isListenerEnabled) {
                /* Receive username */
                if (readCharacter != '\0') {
                    username = username + readCharacter;
                }
                else {
                    startNewGame();
                }
            }
            else {
                /* After game ends */
                if (readCharacter == 'F') {
                    stopSudoku();
                }
                else {
                    /* During game */
                    processMove(readCharacter);
                }
            }
        }
    }

    private void sendAcknowledgement() {
        Serial serial = Serial.getInstance();

        /* Check serial connectivity */
        if (serial.isConnected()) {
            serial.sendCharacter('K');
        }
    }


    private void sendScore(int score) {
        Serial serial = Serial.getInstance();

        /* Check serial connectivity */
        if (serial.isConnected()) {
            serial.sendCharacter((char) score);
        }
    }

    private void sendErrors(List<int[]> errors) {
        Serial serial = Serial.getInstance();

        if (serial.isConnected()) {

            new Thread(() -> {
                for (int[] error : errors) {
                    try {
                        serial.sendCharacter((char) ((char) error[0] + 'A'));
                        serial.sendCharacter((char) ((char) error[1] + '1'));
                        Thread.sleep(1000);
                    } catch (InterruptedException ignored) {}
                }
            }).start();

            /* Notify user errors are being send */
            sudokuView.displayWaitDialog("Sending Errors", "Sending errors to your PIC18F4321", 1000 * errors.size());
        }
    }

    /* -------------------------------------- IGNORED LISTENERS -------------------------------------- */

    /* KeyListener (Ignored) */

    @Override
    public void keyPressed(KeyEvent e) {}

    @Override
    public void keyReleased(KeyEvent e) {}

    /* MouseListener (Ignored) */

    @Override
    public void mousePressed(MouseEvent e) {}

    @Override
    public void mouseReleased(MouseEvent e) {}

    @Override
    public void mouseEntered(MouseEvent e) {}

    @Override
    public void mouseExited(MouseEvent e) {}
}
