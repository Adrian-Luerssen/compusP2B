import controller.SudokuController;
import model.SudokuManager;
import model.TimeManager;
import view.SudokuView;

import javax.swing.*;

public class Main {

    public static void main(String[] args) {

        SwingUtilities.invokeLater(() -> {
            /* model-view-controller */
            TimeManager timeManager = new TimeManager();
            SudokuManager sudokuManager = new SudokuManager();
            SudokuView view = new SudokuView();
            SudokuController controller = new SudokuController(view, sudokuManager, timeManager);

            /* Setup */
            view.registerController(controller);
            timeManager.registerController(controller);

            view.setVisible(true);

            new Thread(() -> {
                while (true) {
                    controller.getSerialInput();
                }
            }).start();
        });
    }
}
