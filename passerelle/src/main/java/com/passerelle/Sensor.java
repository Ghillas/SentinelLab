package com.passerelle;

public interface Sensor {

    public boolean begin();
    public boolean read(Mesure[] out);

}
