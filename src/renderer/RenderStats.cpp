namespace renderstats 
{
    extern int drawCount = 0;
	extern int quadCount = 0;
    extern int cubeCount = 0;
	extern int verticeCount = 0;

    extern void resetStats()
    {
        drawCount = 0;
	    quadCount = 0;
        cubeCount = 0;
	    verticeCount = 0; 
    }
}