package think;

import java.awt.Color;
import java.awt.Graphics;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author strotz
 */
public class Square {
    private int side[], index, rotation;
    
    public Square() {
        side = new int[]{0,0,0,0};
        index = 0;
        rotation = 0;
    }
    
    public Square(int[] a, int b) {
        side = a;
        index = b;
    }
    
    public void reset() {
        side = new int[]{0,0,0,0};
        rotation = 0;
    }
    
    public int getSide(int i) {
        return side[i];
    }
    
    public int getIndex() {
        return index;
    }
    
    public int getRotation() {
        return rotation;
    }
    
    public void setSide(int i, int s) {
        side[i] = s;
    }
    
    public void setIndex(int i) {
        index = i;
    }
    
    public void setRotation(int r) {
        rotation = r;
    }
    
    public int getRotatedSide(int i) {
        int new_idx = i - getRotation();
        while (new_idx < 0)
            new_idx += 4;
        while (new_idx > 3)
            new_idx -= 4;
        
        return getSide(new_idx);
    }
    
    public void draw(Graphics g, int x, int y, boolean sol) {
        g.setColor(Color.BLACK);
        g.drawRect(x, y, 200, 200);
        
        Color arr[] = new Color[]{
            Color.RED,
            Color.BLUE,
            Color.GREEN,
            Color.ORANGE
        };
        
        if (!sol) setRotation(0);
        g.setColor(arr[Math.abs(getRotatedSide(0))-1]);
        g.fillRect(x+20, y, 160, 20);
        
        g.setColor(arr[Math.abs(getRotatedSide(1))-1]);
        g.fillRect(x+180, y+20, 20, 160);
        
        g.setColor(arr[Math.abs(getRotatedSide(2))-1]);
        g.fillRect(x+20, y+180, 160, 20);
        
        g.setColor(arr[Math.abs(getRotatedSide(3))-1]);
        g.fillRect(x, y+20, 20, 160);
        
        g.setColor(Color.BLACK);
        g.fillRect(x, y, 20, 20);
        g.fillRect(x+180, y, 20, 20);
        g.fillRect(x+180, y+180, 20, 20);
        g.fillRect(x, y+180, 20, 20);
        
        g.setColor(Color.BLUE);
        g.drawString(""+getIndex(), x+100,y+100);
        g.drawString("r"+getRotation(), x+100, y+120);
    }
    
    public void draw(Graphics g, int x, int y, boolean sol, Color col) {
        g.setColor(col);
        g.drawRect(x, y, 200, 200);
        
        if (!sol) setRotation(0);
        g.setColor(sol ? new Color(0, 110, 13) : Color.RED );
        g.drawString(""+getRotatedSide(0), x+100, y+20);
        g.drawString(""+getRotatedSide(1), x+180, y+100);
        g.drawString(""+getRotatedSide(2), x+100, y+190);
        g.drawString(""+getRotatedSide(3), x+10, y+100);
        
        g.setColor(Color.BLUE);
        g.drawString(""+getIndex(), x+100,y+100);
        g.drawString("r"+getRotation(), x+100, y+120);
    }
}
