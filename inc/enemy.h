#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

	void initEnemyWithOption(Game* game, int nbEnemy, int enemyByLine, int enemySpeedMove, int nbEnemySolo, int nbBalls, int timeToShoot_ms);
	void initEnemy(Game* game);
	void loadVesselEnemy(Vessel* v, int num);
	void displayEnemyVessel(ListEnemy* list);
	void manageVesselEnemys(Game* game);	
	void manageVesselEnemysSolo(Game* game);
	void destroyEnemy(ListEnemy *list, Enemy* eSupp);
	void deleteEnemy(Enemy* enemy);

#endif