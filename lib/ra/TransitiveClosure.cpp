/*******************************************************

                 Mean Shift Analysis Library
	=============================================


	The mean shift library is a collection of routines
	that use the mean shift algorithm. Using this algorithm,
	the necessary output will be generated needed
	to analyze a given input set of data.

  Mean Shift Image Processor Class:
  ================================

	The following class inherits from the mean shift library
	in order to perform the specialized tasks of image
	segmentation and filtering.
	
	The prototype of the Mean Shift	Image Processor Class
	is provided below. Its definition is provided in
	'msImageProcessor.cc'.

The theory is described in the papers:

  D. Comaniciu, P. Meer: Mean Shift: A robust approach toward feature
									 space analysis.

  C. Christoudias, B. Georgescu, P. Meer: Synergism in low level vision.

and they are is available at:
  http://www.caip.rutgers.edu/riul/research/papers/

Implemented by Chris M. Christoudias, Bogdan Georgescu
********************************************************/


#include "TransitiveClosure.h"

void TransitiveClosure(int width, int height,  int **labels, int* modePointCounts, float *mode,double color_radius,int oldRegionCount, int minRegion){

   
  double color_radius2=color_radius*color_radius;
  int regionCount = oldRegionCount;
// TransitiveClosure
		for(int counter = 0, deltaRegionCount = 1; counter<5 && deltaRegionCount>0; counter++)
		{
			// 1.Build RAM using classifiction structure
			RAList *raList = new RAList [regionCount], *raPool = new RAList [10*regionCount];	//10 is hard coded!
			for(int i = 0; i < regionCount; i++)
			{
				raList[i].label = i;
				raList[i].next = NULL;
			}
			for(int i = 0; i < regionCount*10-1; i++)
			{
				raPool[i].next = &raPool[i+1];
			}
			raPool[10*regionCount-1].next = NULL;
			RAList	*raNode1, *raNode2, *oldRAFreeList, *freeRAList = raPool;
			for(int i=0;i<height;i++) 
				for(int j=0;j<width;j++)
				{
					if(i>0 && labels[i][j]!=labels[i-1][j])
					{
						// Get 2 free node
						raNode1			= freeRAList;
						raNode2			= freeRAList->next;
						oldRAFreeList	= freeRAList;
						freeRAList		= freeRAList->next->next;
						// connect the two region
						raNode1->label	= labels[i][j];
						raNode2->label	= labels[i-1][j];
						if(raList[labels[i][j]].Insert(raNode2))	//already exists!
							freeRAList = oldRAFreeList;
						else
							raList[labels[i-1][j]].Insert(raNode1);
					}
					if(j>0 && labels[i][j]!=labels[i][j-1])
					{
						// Get 2 free node
						raNode1			= freeRAList;
						raNode2			= freeRAList->next;
						oldRAFreeList	= freeRAList;
						freeRAList		= freeRAList->next->next;
						// connect the two region
						raNode1->label	= labels[i][j];
						raNode2->label	= labels[i][j-1];
						if(raList[labels[i][j]].Insert(raNode2))
							freeRAList = oldRAFreeList;
						else
							raList[labels[i][j-1]].Insert(raNode1);
					}
				}
            
				// 2.Treat each region Ri as a disjoint set
				for(int i = 0; i < regionCount; i++)
				{
					RAList	*neighbor = raList[i].next;
					while(neighbor)
					{
						if(color_distance(&mode[3*i], &mode[3*neighbor->label])<color_radius2)
						{
							int iCanEl = i, neighCanEl	= neighbor->label;
							while(raList[iCanEl].label != iCanEl) iCanEl = raList[iCanEl].label;
							while(raList[neighCanEl].label != neighCanEl) neighCanEl = raList[neighCanEl].label;
							if(iCanEl<neighCanEl)
								raList[neighCanEl].label = iCanEl;
							else
							{
								//raList[raList[iCanEl].label].label = iCanEl;
								raList[iCanEl].label = neighCanEl;
							}
						}
						neighbor = neighbor->next;
					}
				}
				// 3. Union Find
				for(int i = 0; i < regionCount; i++)
				{
					int iCanEl	= i;
					while(raList[iCanEl].label != iCanEl) iCanEl	= raList[iCanEl].label;
					raList[i].label	= iCanEl;
				}
				// 4. Traverse joint sets, relabeling image.
				int *modePointCounts_buffer = new int[regionCount];
				memset(modePointCounts_buffer, 0, regionCount*sizeof(int));
				float *mode_buffer = new float[regionCount*3];
				int	*label_buffer = new int[regionCount];

				for(int i=0;i<regionCount; i++)
				{
					label_buffer[i]	= -1;
					mode_buffer[i*3+0] = 0;
					mode_buffer[i*3+1] = 0;
					mode_buffer[i*3+2] = 0;
				}
				for(int i=0;i<regionCount; i++)
				{
					int iCanEl	= raList[i].label;
					modePointCounts_buffer[iCanEl] += modePointCounts[i];
					for(int k=0;k<3;k++)
						mode_buffer[iCanEl*3+k] += mode[i*3+k]*modePointCounts[i];
				}
				int	label = -1;
				for(int i = 0; i < regionCount; i++)
				{
					int iCanEl	= raList[i].label;
					if(label_buffer[iCanEl] < 0)
					{
						label_buffer[iCanEl]	= ++label;

						for(int k = 0; k < 3; k++)
							mode[label*3+k]	= (mode_buffer[iCanEl*3+k])/(modePointCounts_buffer[iCanEl]);

						modePointCounts[label]	= modePointCounts_buffer[iCanEl];
					}
				}
				regionCount = label+1;
				for(int i = 0; i < height; i++)
					for(int j = 0; j < width; j++)
						labels[i][j]	= label_buffer[raList[labels[i][j]].label];

				delete [] mode_buffer;
				delete [] modePointCounts_buffer;
				delete [] label_buffer;

				//Destroy RAM
				delete[] raList;
				delete[] raPool;

				deltaRegionCount = oldRegionCount - regionCount;
				oldRegionCount = regionCount;
				//std::cout<<"Mean Shift(TransitiveClosure):"<<regionCount<<std::endl;
		}
		
		
		// Prune
		{
			int *modePointCounts_buffer = new int[regionCount];
			float *mode_buffer = new float[regionCount*3];
			int	*label_buffer = new int [regionCount];
			int minRegionCount;

			do{
				minRegionCount = 0;
				// Build RAM again
				RAList *raList = new RAList [regionCount], *raPool = new RAList [10*regionCount];	//10 is hard coded!
				for(int i = 0; i < regionCount; i++)
				{
					raList[i].label = i;
					raList[i].next = NULL;
				}
				for(int i = 0; i < regionCount*10-1; i++)
				{
					raPool[i].next = &raPool[i+1];
				}
				raPool[10*regionCount-1].next = NULL;
				RAList	*raNode1, *raNode2, *oldRAFreeList, *freeRAList = raPool;
				for(int i=0;i<height;i++) 
					for(int j=0;j<width;j++)
					{
						if(i>0 && labels[i][j]!=labels[i-1][j])
						{
							// Get 2 free node
							raNode1			= freeRAList;
							raNode2			= freeRAList->next;
							oldRAFreeList	= freeRAList;
							freeRAList		= freeRAList->next->next;
							// connect the two region
							raNode1->label	= labels[i][j];
							raNode2->label	= labels[i-1][j];
							if(raList[labels[i][j]].Insert(raNode2))	//already exists!
								freeRAList = oldRAFreeList;
							else
								raList[labels[i-1][j]].Insert(raNode1);
						}
						if(j>0 && labels[i][j]!=labels[i][j-1])
						{
							// Get 2 free node
							raNode1			= freeRAList;
							raNode2			= freeRAList->next;
							oldRAFreeList	= freeRAList;
							freeRAList		= freeRAList->next->next;
							// connect the two region
							raNode1->label	= labels[i][j];
							raNode2->label	= labels[i][j-1];
							if(raList[labels[i][j]].Insert(raNode2))
								freeRAList = oldRAFreeList;
							else
								raList[labels[i][j-1]].Insert(raNode1);
						}
					}
					// Find small regions
					for(int i = 0; i < regionCount; i++)
						if(modePointCounts[i] < minRegion)
						{
							minRegionCount++;
							RAList *neighbor = raList[i].next;
							int candidate = neighbor->label;
							float minDistance = color_distance(&mode[3*i], &mode[3*candidate]);
							neighbor = neighbor->next;
							while(neighbor)
							{
								float minDistance2 = color_distance(&mode[3*i], &mode[3*neighbor->label]);
								if(minDistance2<minDistance)
								{
									minDistance = minDistance2;
									candidate = neighbor->label;
								}
								neighbor = neighbor->next;
							}
							int iCanEl = i, neighCanEl	= candidate;
							while(raList[iCanEl].label != iCanEl) iCanEl = raList[iCanEl].label;
							while(raList[neighCanEl].label != neighCanEl) neighCanEl = raList[neighCanEl].label;
							if(iCanEl < neighCanEl)
								raList[neighCanEl].label	= iCanEl;
							else
							{
								//raList[raList[iCanEl].label].label	= neighCanEl;
								raList[iCanEl].label = neighCanEl;
							}
						}
						for(int i = 0; i < regionCount; i++)
						{
							int iCanEl	= i;
							while(raList[iCanEl].label != iCanEl)
								iCanEl	= raList[iCanEl].label;
							raList[i].label	= iCanEl;
						}
						memset(modePointCounts_buffer, 0, regionCount*sizeof(int));
						for(int i = 0; i < regionCount; i++)
						{
							label_buffer[i]	= -1;
							mode_buffer[3*i+0]	= 0;
							mode_buffer[3*i+1]	= 0;
							mode_buffer[3*i+2]	= 0;
						}
						for(int i=0;i<regionCount; i++)
						{
							int iCanEl	= raList[i].label;
							modePointCounts_buffer[iCanEl] += modePointCounts[i];
							for(int k=0;k<3;k++)
								mode_buffer[iCanEl*3+k] += mode[i*3+k]*modePointCounts[i];
						}
						int	label = -1;
						for(int i = 0; i < regionCount; i++)
						{
							int iCanEl	= raList[i].label;
							if(label_buffer[iCanEl] < 0)
							{
								label_buffer[iCanEl]	= ++label;

								for(int k = 0; k < 3; k++)
									mode[label*3+k]	= (mode_buffer[iCanEl*3+k])/(modePointCounts_buffer[iCanEl]);

								modePointCounts[label]	= modePointCounts_buffer[iCanEl];
							}
						}
						regionCount = label+1;
						for(int i = 0; i < height; i++)
							for(int j = 0; j < width; j++)
								labels[i][j]	= label_buffer[raList[labels[i][j]].label];

						//Destroy RAM
						delete[] raList;
						delete[] raPool;
						//std::cout<<"Mean Shift(Prune):"<<regionCount<<std::endl;
			}while(minRegionCount > 0);

			delete [] mode_buffer;
			delete [] modePointCounts_buffer;
			delete [] label_buffer;
		}

		
		delete []mode;
		delete []modePointCounts;
		
}

