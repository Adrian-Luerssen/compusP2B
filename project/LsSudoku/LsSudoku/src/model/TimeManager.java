package model;

import model.entity.Time;

public class TimeManager {

    private final Time time;
    private TimeListener timeListener;

    private volatile boolean timeFlag;

    public interface TimeListener {
        void updateTime(String time);
    }

    public TimeManager() {
        time = new Time();
    }

    public void registerController(TimeListener timeListener) {
        this.timeListener = timeListener;
    }

    public void startTimer() {
        time.resetTime();
        timeFlag = true;

        new Thread(() -> {
            while (timeFlag) {
                try {
                    timeListener.updateTime(time.toString());

                    /* Wait 1s */
                    Thread.sleep(1000);
                    time.updateTime();
                } catch (InterruptedException ignored) {}
            }
        }).start();
    }

    public void stopTimer() {
        timeFlag = false;
    }
}
