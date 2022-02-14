package think;

import draw.FrameMain;
import java.awt.Color;
import java.awt.Graphics;
import java.util.HashSet;
import java.util.Set;

/* **Square Side Types **
 * EARTH_NORTH 1
 * EARTH_SOUTH -1
 *
 * SATURN_UP 2
 * SATURN_DOWN -2
 *
 * JUPITER_LIGHT 3
 * JUPITER_DARK -3
 *
 * MARS_TOP 4
 * MARS_BOTTOM -4
 */

public class Board {
    private int size, match_counter;
    private Square square[];
    
    public Board(int a) {
        size = a;
        
        square = new Square[size];
        for (int i = 0; i < size; i++)
           square[i] = new Square(new int[]{0,0,0,0}, i);
    }
    
    public int getSize() {
        return size;
    }
    
    public void init() {
        int[][] total;
        
        total = new int[][]{
            {-3, 1, 3, -4},   // 0
            {3, 2, -4, 2},    // 1
            {4, 1, -1, -2},   // 2
            {-4, -2, -3, -1}, // 3
            {4, -3, -1, 2},   // 4
            {1, -2, -4, 3},   // 5 
            {4, 2, -1, -3},   // 6
            {1, 3, -4, -2},   // 7
            {4, -2, 1, 3}     // 8
        };
                
       square = new Square[size];
       for (int i = 0; i < size; i++)
           square[i] = new Square(total[i], i);
    }
    
    public Square getSquare(int i) {
        return square[i];
    }
    
    public void print(boolean sol) {
        System.out.println("[NUMERICAL REPRESENTATION]");
        for (int i = 0; i < size; i++) {
            System.out.println("[OUTPUT] Square " + i
                    + ":\t"+square[i].getRotatedSide(0)
                    +"\t"+square[i].getRotatedSide(1)
                    +"\t"+square[i].getRotatedSide(2)
                    +"\t"+square[i].getRotatedSide(3));
        }
        
        System.out.println("\n");
        System.out.println("[PHYSICAL REPRESENTATION");
        System.out.println("[INFO] P1 (R0) - Piece 1 with 0 CW rotations");
        for (int d = 0; d < 3; d++) {
            for (int i = 0; i < 3; i++) {
                System.out.print("P" + square[3*d+i].getIndex() + " ("
                        + square[3*d+i].getRotation() + "R)\t\t");
            }
            System.out.println("");
        }
        System.out.println("\n");
    }
    
    public void setSquare(int i, Square s2) {
        for (int d = 0; d < 4; d++)
            square[i].setSide(d, s2.getSide(d));
        
        square[i].setIndex(s2.getIndex());
        square[i].setRotation(s2.getRotation());
    }
    
    void erase() {
    for (int i = 0; i < size; i++)
        square[i].reset();
    }
    
    void erase(int upto) {
        for (int i = upto; i < size; i++)
            square[i].reset();
    }
    
    public void draw(Graphics g, int x1, int y1, boolean sol) {
        g.setColor(Color.BLACK);
        
        for (int y = 0; y < 3; y++)
            for (int x = 0; x < 3; x++)
                if (!sol && !FrameMain.boards.used.contains(y*3+x) || 
                    sol && y*3+x < FrameMain.boards.step || 
                    sol && FrameMain.boards.step == -1)
                    getSquare(y*3+x).draw(g, x1+200*x + (sol ? 0 : 20*x), 
                        y1+200*y + (sol ? 0 : 20*y), sol);
    }
}
