package view;

import controller.SudokuController;
import model.entity.Sudoku;
import model.entity.SudokuCell;
import model.entity.Sudoku.SudokuMode;
import view.dialogs.NotificationDialog;
import view.dialogs.NewGameDialog;
import view.dialogs.SerialDialog;

import javax.swing.*;
import java.awt.*;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

public class SudokuView extends JFrame {

    public static final String APP_NAME = "LSSudoku";

    public static final Dimension FRAME_SIZE = Toolkit.getDefaultToolkit().getScreenSize();
    public static final int FRAME_WIDTH = (int) (FRAME_SIZE.width / 1.5);
    public static final int FRAME_HEIGHT = (int) (FRAME_SIZE.height / 1.2);
    public static final int HEADER_SIZE = 24;
    public static final int FONT_SIZE = 18;
    public static final int ICON_SIZE = 48;

    public static final Color BACKGROUND_COLOR = new Color(255, 255, 255);
    public static final Color FONT_COLOR = new Color(34, 34, 34);
    public static final Color PLACEHOLDER_COLOR = new Color(164, 164, 164);
    public static final Color STATUS_COLOR = new Color(109, 182, 101);
    public static final Color SUDOKU_COLOR = new Color(204, 204, 204);
    public static final Color CELL_COLOR = new Color(255, 255, 255);
    public static final Color SELECTED_CELL_COLOR = new Color(150, 194, 227);
    public static final Color ERROR_COLOR = new Color(241, 120, 120);
    public static final Color BUTTON_COLOR = new Color(204, 204, 204);

    private static final String MESSAGE_STATUS_CONNECTED = "Connected";

    public static final String AC_SERIAL = "AC_SERIAL";
    public static final String AC_GAME = "AC_GAME";
    public static final String AC_SUBMIT = "AC_SUBMIT";

    private static final String IC_GAME = "res/icon/game.png";
    private static final String IC_SERIAL = "res/icon/serial.png";

    private JPanel[][] cellPanel;
    private JPanel currentCellPanel;
    private JPanel mainView;

    private JButton serialButton;
    private JButton newGameButton;
    private JButton submitButton;

    private JLabel statusLabel;
    private JLabel userLabel;
    private JLabel timeLabel;

    public SudokuView() {
        configureView();
        configureFrame();
    }

    /* -------------------------------------- FRAME CONFIGURATION -------------------------------------- */

    private void configureFrame()
    {
        this.pack();
        this.setTitle(APP_NAME);
        this.setSize(FRAME_WIDTH, FRAME_HEIGHT);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setLocationRelativeTo(null);
        this.setResizable(true);
        this.setContentPane(mainView);
    }

    /* -------------------------------------- PANEL CONFIGURATION -------------------------------------- */

    private void configureView() {
        mainView = new JPanel();
        mainView.setLayout(new BorderLayout());

        configureNorth();
        configureCenter();
        configureSouth();

        /* Padding */
        mainView.add(createPadding(BACKGROUND_COLOR, 50, 0), BorderLayout.WEST);
        mainView.add(createPadding(BACKGROUND_COLOR, 50, 0), BorderLayout.EAST);
    }

    private void configureNorth() {
        JPanel northPanel = new JPanel(new BorderLayout());

        /* Center */
        JLabel titleLabel = createLabel(APP_NAME, HEADER_SIZE);
        titleLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        statusLabel = createLabel(MESSAGE_STATUS_CONNECTED, FONT_SIZE);
        statusLabel.setForeground(STATUS_COLOR);
        statusLabel.setVisible(false);
        statusLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        JPanel headerPanel = new JPanel();
        headerPanel.setLayout(new BoxLayout(headerPanel, BoxLayout.Y_AXIS));
        headerPanel.setBackground(BACKGROUND_COLOR);
        headerPanel.add(titleLabel);
        headerPanel.add(statusLabel);
        northPanel.add(headerPanel, BorderLayout.CENTER);

        /* East */
        newGameButton = createIconButton(IC_GAME, AC_GAME);

        JPanel configPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 20, 10));
        configPanel.setBackground(BACKGROUND_COLOR);
        configPanel.add(newGameButton);
        configPanel.add(createPadding(BACKGROUND_COLOR, 10, 0));
        northPanel.add(configPanel, BorderLayout.EAST);

        /* West */
        serialButton = createIconButton(IC_SERIAL, AC_SERIAL);
        JPanel serialPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 20, 10));
        serialPanel.setBackground(BACKGROUND_COLOR);
        serialPanel.add(createPadding(BACKGROUND_COLOR, 10, 0));
        serialPanel.add(serialButton);
        northPanel.add(serialPanel, BorderLayout.WEST);

        /* Padding */
        northPanel.add(createPadding(BACKGROUND_COLOR, 0, 20), BorderLayout.NORTH);
        northPanel.add(createPadding(BACKGROUND_COLOR, 0, 20), BorderLayout.SOUTH);

        mainView.add(northPanel, BorderLayout.NORTH);
    }

    private void configureCenter() {
        JPanel centerPanel = new JPanel(new BorderLayout());

        /* Center */
        JPanel sudokuPanel = new JPanel(new GridLayout(Sudoku.NUM_CELLS, Sudoku.NUM_CELLS));
        sudokuPanel.setBackground(SUDOKU_COLOR);

        cellPanel = new JPanel[Sudoku.NUM_CELLS][Sudoku.NUM_CELLS];
        for (int i = 0; i < Sudoku.NUM_CELLS; i++) {
            for (int j = 0; j < Sudoku.NUM_CELLS; j++) {
                JLabel cellLabel = createLabel("", FONT_SIZE);
                cellLabel.setHorizontalAlignment(JLabel.CENTER);

                cellPanel[i][j] = new JPanel(new BorderLayout());
                cellPanel[i][j].setBackground(CELL_COLOR);

                int top = 1, left = 1, bottom = 1, right = 1;
                if (i == 0) top = 5;
                if (j == 0) left = 5;
                if (i % 3 == 2) bottom = 5;
                if (j % 3 == 2) right = 5;

                cellPanel[i][j].setBorder(BorderFactory.createMatteBorder(top, left, bottom, right, Color.BLACK));
                cellPanel[i][j].setFocusable(false);

                cellPanel[i][j].add(cellLabel, BorderLayout.CENTER);
                sudokuPanel.add(cellPanel[i][j]);
            }
        }
        centerPanel.add(sudokuPanel, BorderLayout.CENTER);

        /* West */
        JPanel verticalLegendPanel = new JPanel(new GridLayout(9, 1));
        verticalLegendPanel.setBackground(SUDOKU_COLOR);

        for (int i = 0; i < 9; i++) {
            JLabel indexLabel = createLabel(String.valueOf(((char) ('A' + i))), FONT_SIZE);
            indexLabel.setHorizontalAlignment(JLabel.CENTER);

            JPanel indexPanel  = new JPanel(new BorderLayout());
            indexPanel.add(indexLabel, BorderLayout.CENTER);
            indexPanel.setBackground(SUDOKU_COLOR);

            /* Padding */
            indexPanel.add(createPadding(SUDOKU_COLOR, 5, 0), BorderLayout.EAST);
            indexPanel.add(createPadding(SUDOKU_COLOR, 5, 0), BorderLayout.WEST);

            verticalLegendPanel.add(indexPanel);
        }
        centerPanel.add(verticalLegendPanel, BorderLayout.WEST);

        /* South */
        JPanel horizontalLegendPanel = new JPanel(new BorderLayout());
        JPanel horizontalLegendInnerPanel =  new JPanel(new GridLayout(1,9));
        horizontalLegendInnerPanel.setBackground(SUDOKU_COLOR);

        for (int i = 0; i < 9; i++) {
            JLabel indexLabel = createLabel(String.valueOf(i + 1), FONT_SIZE);
            indexLabel.setHorizontalAlignment(JLabel.CENTER);

            JPanel indexPanel  = new JPanel(new BorderLayout());
            indexPanel.add(indexLabel, BorderLayout.CENTER);
            indexPanel.setBackground(SUDOKU_COLOR);

            /* Padding */
            indexPanel.add(createPadding(SUDOKU_COLOR, 0,1), BorderLayout.NORTH);
            indexPanel.add(createPadding(SUDOKU_COLOR, 0,1), BorderLayout.SOUTH);

            horizontalLegendInnerPanel.add(indexPanel);
        }
        horizontalLegendPanel.add(horizontalLegendInnerPanel, BorderLayout.CENTER);
        horizontalLegendPanel.add(createPadding(SUDOKU_COLOR, 35,0), BorderLayout.WEST);
        horizontalLegendPanel.add(createPadding(SUDOKU_COLOR, 35,0), BorderLayout.EAST);
        centerPanel.add(horizontalLegendPanel, BorderLayout.SOUTH);

        /* Padding */
        centerPanel.add(createPadding(SUDOKU_COLOR, 35, 0), BorderLayout.EAST);
        centerPanel.add(createPadding(SUDOKU_COLOR, 0, 35), BorderLayout.NORTH);

        mainView.add(centerPanel, BorderLayout.CENTER);
    }


    private void configureSouth() {
        JPanel southPanel = new JPanel(new BorderLayout());

        /* Center */
        JPanel statusPanel = new JPanel(new GridLayout(1,3));
        statusPanel.setBackground(BACKGROUND_COLOR);

        userLabel = createLabel("Username: ", FONT_SIZE);
        JPanel userPanel = new JPanel(new BorderLayout());
        userPanel.setBackground(BACKGROUND_COLOR);
        userPanel.add(userLabel, BorderLayout.CENTER);

        timeLabel = createLabel("", FONT_SIZE);
        timeLabel.setHorizontalAlignment(JLabel.CENTER);
        JPanel timePanel = new JPanel(new BorderLayout());
        timePanel.setBackground(BACKGROUND_COLOR);
        timePanel.add(timeLabel, BorderLayout.CENTER);

        submitButton = createButton("SUBMIT", AC_SUBMIT);
        submitButton.setEnabled(false);
        JPanel submitPanel = new JPanel(new BorderLayout());
        submitPanel.add(submitButton, BorderLayout.CENTER);

        statusPanel.add(userPanel);
        statusPanel.add(timePanel);
        statusPanel.add(submitPanel);
        southPanel.add(statusPanel, BorderLayout.CENTER);

        /* Padding */
        southPanel.add(createPadding(BACKGROUND_COLOR, 50, 0), BorderLayout.WEST);
        southPanel.add(createPadding(BACKGROUND_COLOR, 50, 0), BorderLayout.EAST);
        southPanel.add(createPadding(BACKGROUND_COLOR, 0, 20), BorderLayout.NORTH);
        southPanel.add(createPadding(BACKGROUND_COLOR, 0, 20), BorderLayout.SOUTH);

        mainView.add(southPanel, BorderLayout.SOUTH);
    }

    /* -------------------------------------- COMPONENTS FACTORY -------------------------------------- */

    public static JPanel createPadding(Color color, int width, int height) {
        JPanel border = new JPanel();
        border.setBackground(color);
        border.add(Box.createRigidArea( new Dimension(width, height)));
        return border;
    }

    public static JLabel createLabel(String text, int textSize) {
        JLabel label = new JLabel(text);
        label.setFont(new Font("SansSerif", Font.BOLD, textSize));
        label.setForeground(FONT_COLOR);
        return label;
    }

    public static JButton createButton(String text, String actionCommand) {
        JButton button = new JButton(text);
        button.setBackground(BUTTON_COLOR);
        button.setForeground(FONT_COLOR);
        button.setFont(new Font("SansSerif", Font.BOLD, FONT_SIZE));
        button.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        button.setFocusPainted(false);
        button.setActionCommand(actionCommand);
        return button;
    }

    public static JLabel createIconLabel(String imgPath) {
        JLabel iconLabel = new JLabel(createImage(imgPath));
        iconLabel.setPreferredSize(new Dimension(ICON_SIZE, ICON_SIZE));
        iconLabel.setMaximumSize(iconLabel.getPreferredSize());
        iconLabel.setBackground(BACKGROUND_COLOR);
        iconLabel.setBorder(BorderFactory.createEmptyBorder());
        return iconLabel;
    }

    public static JButton createIconButton(String imgPath, String actionCommand) {
        JButton iconButton = new JButton(createImage(imgPath));
        iconButton.setPreferredSize(new Dimension(ICON_SIZE, ICON_SIZE));
        iconButton.setMaximumSize(iconButton.getPreferredSize());
        iconButton.setBackground(BACKGROUND_COLOR);
        iconButton.setBorder(BorderFactory.createEmptyBorder());
        iconButton.setFocusPainted(false);
        iconButton.setActionCommand(actionCommand);
        return iconButton;
    }

    private static ImageIcon createImage(String imagePath) {
        ImageIcon resource = new ImageIcon(imagePath);
        Image image = resource.getImage().getScaledInstance(ICON_SIZE, ICON_SIZE, Image.SCALE_SMOOTH);
        return new ImageIcon(image);
    }

    /* -------------------------------------- CONTROLLER -------------------------------------- */

    public void registerController(SudokuController controller) {
        for (JPanel[] cellRow : cellPanel) {
            for (JPanel cell : cellRow) {
                cell.addMouseListener(controller);
                cell.addKeyListener(controller);
            }
        }

        newGameButton.addActionListener(controller);
        serialButton.addActionListener(controller);
        submitButton.addActionListener(controller);
    }

    /* -------------------------------------- DIALOGS -------------------------------------- */

    public void displayErrorDialog(String title, String message) {
        new NotificationDialog(this, title, NotificationDialog.IC_ERROR, message).openDialog();
    }

    public void displayCorrectDialog(String title, String message){
        new NotificationDialog(this, title, NotificationDialog.IC_CORRECT, message).openDialog();
    }

    public void displayWaitDialog(String title, String message, int timeout) {
        NotificationDialog waitDialog = new NotificationDialog(this, title, NotificationDialog.IC_WAIT, message);
        waitDialog.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);

        new Timer().schedule(new TimerTask() {
            @Override
            public void run() {
                waitDialog.closeDialog();
            }
        }, timeout);

        waitDialog.openDialog();
    }

    public boolean displaySerialDialog() {
        SerialDialog serialDialog = new SerialDialog(this, "Serial Configuration");
        return serialDialog.openDialog();
    }

    public SudokuMode displayNewGameDialog() {
        NewGameDialog newGameDialog = new NewGameDialog(this, "New Game");
        return (newGameDialog.openDialog()) ? newGameDialog.getSelectedMode() : null;
    }

    /* -------------------------------------- VIEW UPDATE -------------------------------------- */

    /* Sudoku board changes */

    public void paintSudoku(SudokuCell[][] sudoku) {
        for (int i = 0; i < Sudoku.NUM_CELLS; i++) {
            for (int j = 0; j < Sudoku.NUM_CELLS; j++) {
                resetEditableCell(cellPanel[i][j]);
                setCell(cellPanel[i][j], sudoku[i][j].getValue());

                /* Set editable cells */
                if (sudoku[i][j].isEditable()) {
                    setEditableCell(cellPanel[i][j]);
                }
            }
        }
    }

    private void setCell(JPanel cell, int value) {
        for (Component component : cell.getComponents()) {
            if (component instanceof JLabel) {
                ((JLabel) component).setText((value > 0) ? Integer.toString(value) : "");
                return;
            }
        }
    }

    private void setEditableCell(JPanel cell) {
        cell.setFocusable(true);

        for (Component component : cell.getComponents()) {
            if (component instanceof JLabel) {
                component.setForeground(PLACEHOLDER_COLOR);
            }
        }
    }

    private void resetEditableCell(JPanel cell) {
        cell.setFocusable(false);
        cell.setBackground(CELL_COLOR);

        for (Component component : cell.getComponents()) {
            if (component instanceof JLabel) {
                component.setForeground(FONT_COLOR);
            }
        }
    }

    public void setErrorCell(List<int[]> errors) {
        for (int[] error : errors) {
            cellPanel[error[0]][error[1]].setBackground(ERROR_COLOR);
        }
    }

    /* Sudoku current cell */

    public int[]getCurrentCell(JPanel selectedCell) {
        int[] currentCell = new int[2];
        for (int i = 0; i < Sudoku.NUM_CELLS; i++) {
            for (int j = 0; j < Sudoku.NUM_CELLS; j++) {
                if (selectedCell == cellPanel[i][j]) {
                    currentCell[0] = i;
                    currentCell[1] = j;
                    return currentCell;
                }
            }
        }

        return currentCell;
    }

    public void setCurrentCell(int[] currentCell) {
        JPanel newCurrentCellPanel = cellPanel[currentCell[0]][currentCell[1]];

        if (currentCellPanel == null) {
            currentCellPanel = newCurrentCellPanel;
        }

        setCurrentCellColor(newCurrentCellPanel);
    }

    private void setCurrentCellColor(JPanel selectedCell) {
        /* Restore previous cell */
        currentCellPanel.setBackground(CELL_COLOR);

        /* Update current cell */
        currentCellPanel = selectedCell;
        currentCellPanel.setBackground(SELECTED_CELL_COLOR);
        currentCellPanel.requestFocus();
    }

    public void setCurrentCellValue(int value) {
        setCell(currentCellPanel, value);
    }

    /* Text changes */

    public void updateSerialStatus(boolean status) {
        serialButton.setEnabled(!status);
        statusLabel.setVisible(status);
    }

    public void updateTime(String time) {
        timeLabel.setText(time);
    }

    public void setUsername(String username) {
        userLabel.setText("Username: " + username);
    }

    /* Enable/Disable changes */

    public void enableSubmitBtn(boolean enableStatus) {
        submitButton.setEnabled(enableStatus);
    }

    public void enableNewGameBtn(boolean enableStatus) {
        newGameButton.setEnabled(enableStatus);
    }
}