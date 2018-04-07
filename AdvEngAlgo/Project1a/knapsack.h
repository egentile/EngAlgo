// Knapsack class
// Version f08.1

class knapsack
{
   public:
      knapsack(ifstream &fin);
      knapsack(const knapsack &);
      int getCost(int) const;
      int getValue(int) const;
      int getCost() const;
      int getValue() const;
      int getNumObjects() const;
      int getCostLimit() const;
      int getNumSelected();
      void printSolution();
      void select(int);
      void unSelect(int);
      bool isSelected(int) const;
      void exhaustiveKnapsack(int t);

   private:
      int numObjects;
      int costLimit;
      vector<int> value;
      vector<int> cost;
      vector<bool> selected;
      vector<bool> topSelected;
      // vector<int> comboSelected;
      int totalValue;
      int totalCost;
      int currCost;
      int topCost;
      int maxValue;
      int currValue;


};

knapsack::knapsack(ifstream &fin)
// Construct a new knapsack instance using the data in fin.
{
   int n, b, j, v, c;

   fin >> n;  // read the number of objects
   fin >> b;  // read the cost limit

   numObjects = n;
   costLimit = b;

   value.resize(n);
   cost.resize(n);
   selected.resize(n);

   for (int i = 0; i < n; i++)
   {
      fin >> j >> v >> c;
      value[j] = v;
      cost[j] = c;
      unSelect(j);
   }

   totalValue = 0;
   totalCost = 0;
}

knapsack::knapsack(const knapsack &k)
// Knapsack copy constructor.
{
   int n = k.getNumObjects();

   value.resize(n);
   cost.resize(n);
   selected.resize(n);
   numObjects = k.getNumObjects();
   costLimit = k.getCostLimit();

   totalCost = 0;
   totalValue = 0;

   for (int i = 0; i < n; i++)
   {
      value[i] = k.getValue(i);
      cost[i] = k.getCost(i);
      if (k.isSelected(i))
	 select(i);
      else
	 unSelect(i);
   }
}

int knapsack::getNumObjects() const
{
   return numObjects;
}

int knapsack::getCostLimit() const
{
   return costLimit;
}


int knapsack::getValue(int i) const
// Return the value of the ith object.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getValue");

   return value[i];
}

int knapsack::getCost(int i) const
// Return the cost of the ith object.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getCost");

   return cost[i];
}

int knapsack::getCost() const
// Return the cost of the selected objects.
{
  int collectiveCost = 0;
  for(int i = 0; i < getNumObjects(); i++)
  {
    if(isSelected(i))
    {
      collectiveCost = collectiveCost + getValue(i);
    }
  }
   return collectiveCost;
}

int knapsack::getValue() const
// Return the value of the selected objects.
{
   int collectiveValue= 0;
   for(int i = 0; i < getNumObjects(); i++)
   {
     if(isSelected(i))
     {
       collectiveValue = collectiveValue + getValue(i);
     }
   }
   return collectiveValue;
}

// returns number of currently selected objects
int knapsack::getNumSelected()
{
  int count = 0;

  for(int i = 0; i < numObjects; i++)
  {
    if (isSelected(i))
    {
      count++;
    }
  }

  return count;
}

ostream &operator<<(ostream &ostr, const knapsack &k)
// Print all information about the knapsack.
{
   cout << "------------------------------------------------" << endl;
   cout << "Num objects: " << k.getNumObjects() << " Cost Limit: " << k.getCostLimit() << endl;

   int totalValue = 0;
   int totalCost = 0;

   for (int i = 0; i < k.getNumObjects(); i++)
   {
      totalValue += k.getValue(i);
      totalCost += k.getCost(i);
   }

   cout << "Total value: " << totalValue << endl;
   cout << "Total cost: " << totalCost << endl << endl;

   for (int i = 0; i < k.getNumObjects(); i++)
      cout << i << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

   cout << endl;

   return ostr;
}

void knapsack::printSolution()
// Prints out the solution.
{

  // ofstream outputFile;
  // string outName = fileName + ".txt";
  // outputFile.open (outName);
  //////// sub in outputFile for cout



  // unselects all
  for( int k = 0; k < numObjects; k++)
  {
    unSelect(k);
  }

  cout << "unselected all" << endl;

  // cout << "top selected size: " << topSelected.size() << endl;
  // reselects topSelected
  for( int m = 0; m < getNumObjects(); m++)
  {
    if(topSelected[m] == true)
    {
      select(m);
    cout << "selecting: " << m << endl;
    }
  }

   cout << "------------------------------------------------" << endl;

   cout << "Total value: " << getValue() << endl;
   cout << "Total cost: " << getCost() << endl << endl;

   // Print out objects in the solution
   for (int i = 0; i < getNumObjects(); i++)
      if (isSelected(i))
	 cout << i << "  " << getValue(i) << " " << getCost(i) << endl;

   cout << endl;
}

ostream &operator<<(ostream &ostr, vector<bool> v)
// Overloaded output operator for vectors.
{
   for (int i = 0; i < v.size(); i++)
      cout << v[i] << endl;

   return ostr;
}

void knapsack::select(int i)
// Select object i.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::Select");

   if (selected[i] == false)
   {
      selected[i] = true;
      currCost = currCost + getCost(i);
      currValue = currValue + getValue(i);
   }
}

void knapsack::unSelect(int i)
// unSelect object i.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::unSelect");

      if (selected[i] == true)
      {
         selected[i] = false;
         currCost = currCost - getCost(i);
         currValue = currValue - getValue(i);
      }
}

bool knapsack::isSelected(int i) const
// Return true if object i is currently selected, and false otherwise.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getValue");

   return selected[i];
}

void knapsack::exhaustiveKnapsack(int t)
// Exhaustive knapsack calculation
{


    int numberObjects = getNumObjects();
    int maxValue = 0;

    // iterator for each combo (excepting unselecting all objects)
    for (int s = 1; s < (1 << numberObjects); s++)
    {


        // generates each of the combos
        // cout << "combo genreated: { " ;
        for (int e = 0; e < numberObjects; e++)
        {
            if(s & (1 << e))
            {
              // comboSelected.push_back(index[e]);
              select(e);
              // cout << e;
            }
        }

      //  cout << "value is: " << getValue() << endl;
        // cout << endl << endl;

        // if total value is bigger than previous total value
        if(getValue() > maxValue && getCost() <= costLimit)
        {

          // cout << "current combo: " << "{ ";

          topSelected.resize(0);
          //topSelected is a bool vector

          for (int x = 0; x < numberObjects; x++)
          {
            // // cout << comboSelected[x];
            if (isSelected(x))
            {
              topSelected.push_back(true);
            }

            else
            {
              topSelected.push_back(false);
            }

          }

          cout << endl;

            //  cout << "entering new top value" << endl;
            //  cout << "totalValue == " << totalValue << " > " << "maxValue ==" << maxValue << endl;
            //  cout << "totalCost == " << totalCost << " <= " << "costLimit == " << costLimit;
            //  cout << "current combo" << endl;

            //  for(int y = 0; y < topSelected.size(); y++)
            //  {
            //    cout << topSelected[y] << "    " << value[topSelected[y]] << "    " << cost[topSelected[y]] << endl;
            //  }

            cout << "current total cost" << totalCost << endl;

            cout << "curretn total value " << totalValue << endl << endl;


            cout << "new topSelected size == " << topSelected.size() << endl;

            cout << " }" << endl;

            maxValue = currValue;
            topCost = currCost;

        }

      // // reset comboselected vector
      // comboSelected.resize(0);

      for (int z = 0; z < numberObjects; z++)
      {
        unSelect(z);
      }
    }

    for( int k = 0; k < numObjects; k++)
    {
      unSelect(k);
    }

    cout << "unselected all" << endl;

    // cout << "top selected size: " << topSelected.size() << endl;
    // reselects topSelected
    for( int m = 0; m < getNumObjects(); m++)
    {
      if(topSelected[m] == true)
      {
        select(m);
      cout << "selecting: " << m << endl;
      }
    }

      cout << endl << endl;
      cout << "max cost allowed: " << costLimit << endl;
      cout << "total cost calculated: " << getCost() << endl;
      cout << "max value calculated: " << getValue() << endl;

      cout << "combo used: " << endl;
      cout << endl;
      cout << "index value cost" << endl;

      for(int y = 0; y < getNumObjects(); y++)
      {
        if(isSelected(y))
        {
          cout << topSelected[y] << "    " << value[topSelected[y]] << "    " << cost[topSelected[y]] << endl;
        }
      }



}
