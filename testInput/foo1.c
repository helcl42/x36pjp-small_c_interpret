void main()
{
	int a;
	int b;
	
	a = 5;
	b = 43;

	if(a  >  b)
	{
		printf(a + 6 / 4);
	} 
	else
	{
		printf(2222);
	}

	printf(4333);
	printf(b);

	int fact;
	int n;
	
	g = 5;
	fact = 1;

    	while ( 1 < g ) 
	{
		fact = fact * g;
		g = g - 1;		
		printf(fact);
    	}	
		
	printf(4333);
	printf(5433);

	
	int sum;
	int i;
	sum = 9;
	a = 0;
  	b = 1;
	n = 10;
      
  	for (i = 0 ; i < n; i++)
  	{
    		sum = a + b;
    		a = b;
    		b = sum;  			
		printf(a);
	}	
}
