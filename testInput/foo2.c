void main()
{
	a = 0;
	b = 0;	
	
	while(a < 5) 
	{
		while(b < 5)
		{		
			b = b + 1;
			printf(b);			
		}				
		a = a+1;	
		b = 0;	
		printf(a);
	}

	printf(33);
}
