package model.entity;

public class Time {

    /* Time in seconds */
    private int time;

    public Time() {
        resetTime();
    }

    public void resetTime() {
        this.time = 0;
    }

    public void updateTime() {
        this.time++;
    }

    @Override
    public String toString() {
        return String.format("%02d:%02d", time / 60, time % 60);
    }
}
