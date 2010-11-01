#include "game.h"
#include "unit.h"

Game::Game(int numPlayers, int numRows, int numCols)
	: numPlayers(numPlayers), rows(numRows), cols(numCols)
{
	// Allocate grid
	*cells = (GridCell*)malloc(rows * sizeof(GridCell*));
	for(int j = 0; j < rows; j++){
		cells[j] = (GridCell*)malloc(cols * sizeof(GridCell*));
	}

	// Initialize grid
	for(int x = 0; x < rows; x++){
		for(int y = 0; y < cols; y++){
			cells[x][y].setCoords(x, y);
			cells[x][y].setOwner(NULL);
		}
	}
}

Game::~Game(){
	for(int x = 0; x < rows; x++){
		delete cells[x];
	}
	delete cells;
}

void Game::addUnit(Unit *u){
    u->setId(units.size());
    units.push_back(u);
}

GridCell * Game::getCell(int row, int col){
    return &(cells[row][col]);
}

/* This should translate fp coordinates to the nearest cell.
 * This function is incomplete.
 */
GridCell * Game::getCell(float r, float c){
	return getCell((int)(getHeight()*r), (int)(getWidth()*c));
}


CIwArray<Unit*>* Game::getUnitsNear(int row, int col, int radius){
    
	CIwArray<Unit*> *units = new CIwArray<Unit*>();
    for (int r = row-radius; r <= row+radius; r++) {
        for (int c = col-radius; c <= col+radius; c++) {
            if (r >= 0 && r <= rows && c >= 0 && c <= cols) {
				
				
				CIwArray<Unit*> cellUnits = getCell(r, c)->getUnits();
				for (CIwArray<Unit*>::iterator it = cellUnits.begin(); it != cellUnits.end(); ++it) {
					units->push_back(*it);
				}
            }
        }
    }
    
    return units;
}

void Game::tick(){
	CIwArray<GridCell*> *changes = new CIwArray<GridCell*>();
	//for(Unit * u = units.begin(); *u <= units.end(); u++){
		
	//}
}

int Game::getHeight(){
	return rows;
}

int Game::getWidth(){
	return cols;
}