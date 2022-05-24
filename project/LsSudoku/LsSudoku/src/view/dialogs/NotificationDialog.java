package view.dialogs;

import javax.swing.*;
import java.awt.*;

import static view.SudokuView.*;

public class NotificationDialog extends AbstractDialog
{
    public static final String IC_ERROR = "res/icon/error.png";
    public static final String IC_CORRECT = "res/icon/correct.png";
    public static final String IC_WAIT = "res/icon/wait.png";

    private final String icon;
    private final String message;

    public NotificationDialog(Frame owner, String title, String icon, String message)
    {
        super(owner, title);

        this.icon = icon;
        this.message = message;

        configureView();
        this.pack();
        this.setLocationRelativeTo(owner);
    }

    /* -------------------------------------- PANEL CONFIGURATION -------------------------------------- */

    protected void configureView()
    {
        JPanel dialogPanel = new JPanel(new BorderLayout());

        /* Center */
        JLabel iconLabel = createIconLabel(icon);
        JLabel messageLabel = createLabel(this.message, FONT_SIZE);

        JPanel messagePanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 20, 0));
        messagePanel.setBackground(BACKGROUND_COLOR);
        messagePanel.add(iconLabel);
        messagePanel.add(messageLabel);

        dialogPanel.add(messagePanel, BorderLayout.CENTER);

        /* Padding */
        dialogPanel.add(createPadding(BACKGROUND_COLOR, 10, 0), BorderLayout.WEST);
        dialogPanel.add(createPadding(BACKGROUND_COLOR, 10, 0), BorderLayout.EAST);
        dialogPanel.add(createPadding(BACKGROUND_COLOR, 0, 10), BorderLayout.NORTH);
        dialogPanel.add(createPadding(BACKGROUND_COLOR, 0, 10), BorderLayout.SOUTH);

        this.add(dialogPanel);
    }
}
