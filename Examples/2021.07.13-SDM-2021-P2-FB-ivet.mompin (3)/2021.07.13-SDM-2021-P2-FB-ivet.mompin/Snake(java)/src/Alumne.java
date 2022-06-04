import com.fazecast.jSerialComm.SerialPort;

public class Alumne {
    public static final int SNAKE_STEP_DELAY = 150; // Time to wait between snake steps in milliseconds
    public static final int TIME_WAIT_MS = 100; // Time to wait before calling "getInput()" in milliseconds
    public static boolean GameRunning = false;
    public static boolean TimeIsUp = false;
    public static boolean now = false;
    public static boolean printOnce = false;
    public static boolean nameGot = false;
    public static byte[] content = new byte[1];
    public static byte[] scoresend = new byte[1];
    public static String username = "";
    public static int seconds = 0;

    public static void sendScore(Snake snake, SerialPort serialPort) {
        int score = snake.getScore();
        scoresend[0]=(byte)score;
        serialPort.writeBytes(scoresend,1);

    }

    public static void getInput(Snake snake, SerialPort serialPort) {
        if (serialPort.bytesAvailable() >= 1 && !GameRunning && !now) {
            serialPort.readBytes(content, 1);

            if (((char) content[0]) == 'Y') {

                now = true;

            }
        } if (serialPort.bytesAvailable() >= 1 && !GameRunning && now) {
            byte[] readBuffer = new byte[10];
            int valueRead = serialPort.readBytes(readBuffer, 10);


            for (int j = 0; j < 10; j++) {
                if (((char) content[0]) != '\0') {
                    username += (char) (readBuffer[j]);

                }
            }
            nameGot = true;


            if (nameGot) {
                snake.startGame();
                GameRunning = true;
                if (!printOnce) {
                    snake.setName(username);
                    System.out.println(username);
                    printOnce = true;
                }
            }
        }
           while(GameRunning && printOnce) {

                if (serialPort.bytesAvailable() >= 1) {
                    serialPort.readBytes(content, 1);

                    if(((char) content[0]) == '+'){
                        seconds++;

                    }else if (((char) content[0]) == 'R') {
                        snake.right();
                    } else if (((char) content[0]) == 'L') {
                        snake.left();
                    } else if (((char) content[0]) == 'U') {
                        snake.up();
                    } else if (((char) content[0]) == 'D') {
                        snake.down();
                    }

                }
               snake.setTime(seconds);
            }
        }


    public static void gameOver(Snake snake, SerialPort serialPort) {
        int finish=250;
        content[0] = (byte) finish;
        serialPort.writeBytes(content,1);
        snake.createInitSnake();
    }
}
