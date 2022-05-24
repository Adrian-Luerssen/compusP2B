package view.dialogs;

import model.entity.Sudoku.SudokuMode;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import static view.SudokuView.*;

public class NewGameDialog extends AbstractDialog implements ActionListener
{
    private static final String AC_PLAY = "AC_PLAY";
    private static final String AC_MODE = "AC_MODE";

    private JComboBox<SudokuMode> modesComboBox;
    private SudokuMode selectedMode;

    private boolean isClosed;

    public NewGameDialog(Frame owner, String title)
    {
        super(owner, title);
        this.isClosed = false;
        this.selectedMode = SudokuMode.EASY;

        configureView();
        this.pack();
        this.setLocationRelativeTo(owner);
    }

    public SudokuMode getSelectedMode() {
        return selectedMode;
    }

    /* -------------------------------------- PANEL CONFIGURATION -------------------------------------- */

    @Override
    protected void configureView()
    {
        JPanel dialogPanel = new JPanel(new BorderLayout());

        /* Center */
        JLabel modeLabel = createLabel("Mode:", FONT_SIZE);
        JButton playButton = createButton("NEW GAME", AC_PLAY);
        playButton.addActionListener(this);

        modesComboBox = new JComboBox<>(SudokuMode.values());
        modesComboBox.addActionListener(this);
        modesComboBox.setActionCommand(AC_MODE);
        modesComboBox.setSelectedIndex(1);

        JPanel serialPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 20, 0));
        serialPanel.setBackground(BACKGROUND_COLOR);
        serialPanel.add(modeLabel);
        serialPanel.add(modesComboBox);
        serialPanel.add(playButton);

        dialogPanel.add(serialPanel, BorderLayout.CENTER);

        /* Padding */
        dialogPanel.add(createPadding(BACKGROUND_COLOR, 10, 0), BorderLayout.WEST);
        dialogPanel.add(createPadding(BACKGROUND_COLOR, 10, 0), BorderLayout.EAST);
        dialogPanel.add(createPadding(BACKGROUND_COLOR, 0, 10), BorderLayout.NORTH);
        dialogPanel.add(createPadding(BACKGROUND_COLOR, 0, 10), BorderLayout.SOUTH);

        this.add(dialogPanel);
    }
    /* -------------------------------------- VIEW UPDATE -------------------------------------- */

    @Override
    public boolean openDialog() {
        super.openDialog();

        return isClosed;
    }

    /* -------------------------------------- CONTROLLER -------------------------------------- */

    @Override
    public void actionPerformed(ActionEvent e)
    {
        switch (e.getActionCommand())
        {
            case AC_MODE -> selectedMode = (SudokuMode) modesComboBox.getSelectedItem();
            case AC_PLAY ->  {
                isClosed = true;
                closeDialog();
            }
        }
    }
}
