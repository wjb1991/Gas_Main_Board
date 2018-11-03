
//==================================================================================================
//| 文件名称 | Pub_Nihe.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | 最小二乘法数据拟合
//|--------- |--------------------------------------------------------------------------------------
//| 运行环境 | 所有C/++语言编译器，包括单片机编译器
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright2007, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2007.04.10  | gao       | 初版
//|----------|-------------|-----------|------------------------------------------------------------
//|  V1.1    | 2007.04.15  | gao       | 增加拟合精度选项,内存占用减少到原来的一半
//==================================================================================================

#define  NIHE_GLOBALS
#include "Pub_Nihe.h"

//==================================================================================================
//| 函数名称 | NiHe1()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 曲线拟合函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | x:  x数据组指针  
//|          | y:  y数据组指针 
//|          | n:  数据个数 
//|          | a:  拟合结果存放数组指针 
//|          | N:  拟合阶次 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | a:  拟合结果存放数组指针
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | gao,06.06.12
//==================================================================================================

#if NIHE1_EN
void NiHe1(float *x, float *y, int n, float *a, int N)
{   
	int i,j,k;  
	int N1=N+1; //N表示拟合多项式的最高次数,N1是对应系数求解矩阵的行数
	
#if DOUBLE_PRECISION==1
  double A[N_MAX+1][N_MAX+2]={0};  //双精度数据类型
	double m[N_MAX*2+1]={0};
	double h;
#else
	float A[N_MAX+1][N_MAX+2]={0};   //单精度数据类型
	float m[N_MAX*2+1]={0};
	float h;
#endif
	
	//*********************************************************
	//**************以下求出系数矩阵***************************
	//*********************************************************
	//求m系数,纵向求解
	for(i=0;i<=2*N;i++)  m[i]=0;
	for(j=0;j<n;j++)
		for(i=0,h=1;i<=2*N;i++)  
		{
			m[i]+= h; 
			h=h*x[j]; 
		}
		
    //赋值给数组A
	for(i=0;i<N1;i++)
		for(j=0;j<N1;j++)
			A[i][j]=m[i+j]; 
		
    //求b系数,纵向求解
	for(i=0;i<N1;i++) A[i][N1]=0;
	for(j=0;j<n;j++)
		for(i=0,h=1;i<N1;i++)  
		{
			A[i][N1]+= h*y[j]; 
			h=h*x[j]; 
		}
		
    //*********************************************************
	// 以下用于把增广矩（A|b）阵中的A变成对角矩阵A’，同时b变成b’
    // ********************************************************	
	for(k=0;k<N1;k++)                      //N1为维数，也是重复次数
	{
	    //保存系数，其中数组m重用以减少内存使用量
        for(i=0;i<N1;i++)  m[i]=A[i][k];   

	    //数组每行均除以该行的第k个数,i为行，j为列
	    for(i=0;i<N1;i++)
			for(j=0;j<=N1;j++)             	
				A[i][j]=A[i][j]/m[i];
					
		//第2，3，4，行分别减去第1行			
	    for(i=0;i<N1;i++)
			for(j=0;j<=N1;j++)              
				if(i!=k)    A[i][j]=A[i][j]-A[k][j];
	}
		
	//*********************************************************
	//以下用于把增广矩（A’|b’）阵中的A’变成单位矩阵E,
	//*********************************************************

	//保存系数，其中数组m重用以减少内存使用量
    for(i=0;i<N1;i++)  m[i]=A[i][i];    
    
	//第i行除以该行第i个数		
	for(i=0;i<N1;i++)
		for(j=0;j<=N1;j++)      
            A[i][j]=A[i][j]/m[i];           
		
	
	//*********************************************************
	//以下赋值给多项式的系数从a[i],i=1,2,3...N 
	//*********************************************************
	for(i=0;i<=N;i++)  a[i]=(float)A[i][N1];  //强制转换为单精度数据类型	
}
#endif



//==================================================================================================
//| 函数名称 | NiHe2()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 双气体标定函数函数(加权)
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | x:  x数据组指针  
//|          | y:  y数据组指针 
//|          | w:  权重系数据组指针 
//|          | n:  数据个数 
//|          | a:  拟合结果存放数组指针 
//|          | N:  拟合阶次 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | a:  拟合结果存放数组指针
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | gao,06.06.12
//==================================================================================================


#if NIHE2_EN
void NiHe2(float *x, float *y, float *w, int n, float *a, int N)
{
	int i,j,k;  
	int N1=N+1; //N表示拟合多项式的最高次数,N1是对应系数求解矩阵的行数
	
#if DOUBLE_PRECISION==1
  double A[N_MAX+1][N_MAX+2]={0};  //双精度数据类型
	double m[N_MAX*2+1]={0};
	double h;
#else
	float A[N_MAX+1][N_MAX+2]={0};   //单精度数据类型
	float m[N_MAX*2+1]={0};
	float h;
#endif
	
	//*********************************************************
	//**************以下求出系数矩阵***************************
	//*********************************************************
	//求m系数,纵向求解
	for(i=0;i<=2*N;i++)  m[i]=0;
	for(j=0;j<n;j++)
		for(i=0,h=1;i<=2*N;i++)  
		{
			m[i]+= h*w[j];           //...此处加权重系数w
			h=h*x[j]; 
		}
		
    //赋值给数组A
	for(i=0;i<N1;i++)
		for(j=0;j<N1;j++)
			A[i][j]=m[i+j]; 
		
    //求b系数,纵向求解
	for(i=0;i<N1;i++) A[i][N1]=0;
	for(j=0;j<n;j++)
		for(i=0,h=1;i<N1;i++)  
		{
			A[i][N1]+= h*y[j]*w[j];   //...此处加权重系数w
			h=h*x[j]; 
		}
		
    //*********************************************************
	// 以下用于把增广矩（A|b）阵中的A变成对角矩阵A’，同时b变成b’
    // ********************************************************	
	for(k=0;k<N1;k++)                      //N1为维数，也是重复次数
	{
	    //保存系数，其中数组m重用以减少内存使用量
       for(i=0;i<N1;i++)  m[i]=A[i][k];   

	    //数组每行均除以该行的第k个数,i为行，j为列
	    for(i=0;i<N1;i++)
			for(j=0;j<=N1;j++)             	
				A[i][j]=A[i][j]/m[i];
					
		//第2，3，4，行分别减去第1行			
	    for(i=0;i<N1;i++)
			for(j=0;j<=N1;j++)              
				if(i!=k)    A[i][j]=A[i][j]-A[k][j];
	}
		
	//*********************************************************
	//以下用于把增广矩（A’|b’）阵中的A’变成单位矩阵E,
	//*********************************************************

	//保存系数，其中数组m重用以减少内存使用量
    for(i=0;i<N1;i++)  m[i]=A[i][i];    
    
	//第i行除以该行第i个数		
	for(i=0;i<N1;i++)
		for(j=0;j<=N1;j++)      
            A[i][j]=A[i][j]/m[i];           
		
	
	//*********************************************************
	//以下赋值给多项式的系数从a[i],i=1,2,3...N 
	//*********************************************************
	for(i=0;i<=N;i++)  a[i]=(float)A[i][N1];  //强制转换为单精度数据类型	
}
#endif



//==================================================================================================
//| 函数名称 | s_fx()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获取拟合函数f(x)=a[0] + a[1]*x + ... +a[N]*x^N在点x处的值
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | a:  拟合结果存放数组指针  
//|          | N:  拟合阶次 
//|          | x:  所求点参数(实数) 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | a:  拟合结果存放数组指针
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | gao,06.06.12
//==================================================================================================

float s_fx(float *a, int N, float x)
{
	int i;
#if DOUBLE_PRECISION==1            //双精度数据类型
	double h=1,s=0; 
#else
  float  h=1,s=0;                  //单精度数据类型
#endif

	for(i=0;i<=N;i++) 
	{
	    s+=a[i]*h;
	  	h=h*x;
	}
	return((float)s);
}

