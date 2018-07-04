#include <iostream>
//#include <boost/assign/list_of.hpp>
#include <pthread.h>
#include <algorithm>
#include <string>
#include <vector>

/**
 * template for const vector initialisation
 * @see COFU (construct on first use)
 * @params vector + arguments to insert by overloading operator '()'
 * @return const vector
 */
template <typename T>
struct decVec : public std::vector<T>
{ 
   decVec(T & ptr_temp)
   {
      (*this)(ptr_temp);
   }
   
   decVec & operator()(T & ptr_temp)
   {
      ptr_temp = ((unsigned int)(ptr_temp * 100)) / 100;
      this->push_back(ptr_temp);
      return *this;
   }
};

class CNB
{
  private: 
   //Separator for printData method
   std::string gzk_separator;
   //Standard Buy / Sell - Europe
   static const double g8k_coinbase_tx_buy = 1.49, g8k_coinbase_tx_sell = 1.49;
   //Bank transfers (SEPA)  - in / out*
   static const double g8k_coinbase_bkt_sepa_out = 0.15, g8k_coinbase_bkt_sepa_in = 0.00;
   //Buy / Sell value of stock price
   double g8_in, g8_out;
   //Amount of Buy / Sell stocks
   unsigned int gu4_amt_in, gu4_amt_out;
   //Buy / Sell value taxation based on stock price
   double g8_hot_in, g8_hot_out;
   //Stock price in/out after taxation
   double g8_hot_stock_in, g8_hot_stock_out;
   //Stock group price
   double g8_sgp_in, g8_sgp_out;
   //Stock group price after taxation
   double g8_sgp_tx_in, g8_sgp_tx_out;
   //Stock group price taxation value (without SEPA bank transfert)
   double g8_sqp_tx_val_in, g8_sqp_tx_val_out;
   //Stock group price taxation value over the whole stocks
   double g8_sgp_tax_in, g8_sgp_tax_out;
   //Stock group price final taxation value
   double g8_sqp_fin_tx_val_in, g8_sqp_fin_tx_val_out;
   //Final Stock group price
   double g8_final_sqp_in, g8_final_sqp_out;
   //Bonus
   double g8_gain;
   //Stock gap
   double g8_gap;
   //Stock group gap
   double g8_gap_sqp;
   //Vector of all values
   std::vector<double> gv8_values;
   //For loading
   static bool sg1_load;
   
   /**
    * foo to trunc old C
    * @see to 1.xx 2 decimals
    * @param p8r_val double reference
    * @return void
    */
   double oldCtrunc(const double & p8r_val)
   { 
      return ((unsigned int)(p8r_val * 100)) / 100;
   }
   
   /**
    * method reload dynamicly values
    * @see it will restore the value as contained inside the gv8_values with trunc
    * @return void
    */
   void reloadVals()
   {
      
   }
   
   /**
    * method reload dynamicly values
    * @see it will load the value inside the gv8_values with trunc
    * @return void
    */
   void loadVals()
   {
      
      gv8_values = decVec<double>(g8_in)(g8_out)(g8_hot_in)(g8_hot_out)(g8_hot_stock_in)(g8_hot_stock_out)
                                 (g8_sgp_in)(g8_sgp_out)(g8_sgp_tx_in)(g8_sgp_tx_out)(g8_sqp_tx_val_in)
                                 (g8_sqp_tx_val_out)(g8_sgp_tax_in)(g8_sgp_tax_out)(g8_sqp_fin_tx_val_in)
                                 (g8_sqp_fin_tx_val_out)(g8_final_sqp_in)(g8_final_sqp_out)(g8_gain)(g8_gap)(g8_gap_sqp);
   }

  public:
   CNB():g8_hot_in(0.00), g8_hot_out(0.00), g8_hot_stock_in(0.00), g8_hot_stock_out(0.00), 
   g8_sgp_in(0.00), g8_sgp_out(0.00), g8_sgp_tx_in(0.00), g8_sgp_tx_out(0.00), g8_sqp_tx_val_in(0.00), 
   g8_sqp_tx_val_out(0.00), g8_final_sqp_in(0.00), g8_final_sqp_out(0.00), g8_in(0.00), g8_out(0.00),
   gu4_amt_in(0), gu4_amt_out(0), g8_sqp_fin_tx_val_in(0.00), g8_sqp_fin_tx_val_out(0.00), g8_gain(0.00),
   g8_gap(0.00), g8_gap_sqp(0.00), g8_sgp_tax_in(0.00), g8_sgp_tax_out(0.00)
   {
      gzk_separator = std::string(40, 0x2D);
   };
   
   virtual ~CNB(){};
   
   /**
    * @see function inCNB for computing real final stock group price after taxation of CNB
    * @param p8r_stockPrx const double reference stock price
    * @param p4r_stockNb const unsigned integer reference stock amount
    * @return g8_final_sqp_in Final Stock group price for INCOMING
    */
   double inCNB(const double & p8r_stockPrx, const unsigned int & p4r_stockNb);
   
   /**
    * @see method which print all the data related to an incoming stock price computing
    * @return void (internally use ostream)
    */
   void printData_in() const;
   
   /**
    * @see method which simulate gains over uprising stocks prices at sell
    * @param const double reference increment
    * @param const unsigned int reference cycle
    * @param const double reference gain limit
    * @return void
    */
   void simGain(const double & p8r_inc, const unsigned int & pu4r_cycle, const double & p8r_gainLim);
   
   /**
    * @see method which check gain on in/out parameters
    * @param p8r_in const double reference buy value
    * @param p8r_out const double reference sell value
    * @param p4r_amt const unsigned int reference stock amount
    * @return double gain
    */
   double fastGain(const double & p8r_in, const double & p8r_out, const unsigned int & p4r_amt);
};

/**
 * @see method which check gain on in/out parameters
 * @param p8r_in const double reference buy value
 * @param p8r_out const double reference sell value
 * @param p4r_amt const unsigned int reference stock amount
 * @return double gain
 */
double CNB::fastGain(const double & p8r_in, const double & p8r_out, const unsigned int & p4r_amt)
{
   g8_sgp_tax_in = (((p8r_in * p4r_amt) / 100) * g8k_coinbase_tx_buy);
   g8_sgp_tax_out = (((p8r_out * p4r_amt) / 100) * g8k_coinbase_tx_sell);
   
   return oldCtrunc((double)((p8r_out * p4r_amt - p8r_in * p4r_amt) - (g8_sgp_tax_out + g8_sgp_tax_in + g8k_coinbase_bkt_sepa_out + g8k_coinbase_bkt_sepa_in)));
}

/**
 * @see method which print all the data related to an incoming stock price computing
 * @return void (internally use ostream)
 */
void CNB::printData_in() const
{
   //General info
   std::cout << gzk_separator << std::endl;
   std::cout << "Standard buy Europe taxation : " << g8k_coinbase_tx_buy << "%" << std::endl;
   std::cout << "Bank transfert SEPA in : " << g8k_coinbase_bkt_sepa_out << "€" << std::endl;
   //Stock info
   std::cout << gzk_separator << std::endl;
   std::cout << "Initial stock price : " << g8_in << "€" << std::endl;
   std::cout << "CNB taxation value applicated : " << g8_hot_in << "€" << std::endl;
   std::cout << "Ending stock price : " << g8_hot_stock_in << "€" << std::endl;
   //Stock group info
   std::cout << gzk_separator << std::endl;
   std::cout << "Stock amount : " << gu4_amt_in << " parts" << std::endl;
   std::cout << "CNB taxation value applicated (without bank transfert SEPA in) : " << g8_sqp_tx_val_in << "€" << std::endl;
   std::cout << "CNB final taxation value : " << g8_sqp_fin_tx_val_in << "€" << std::endl;
   std::cout << "No tax stock group price : " << g8_sgp_in << "€" << std::endl;
   std::cout << "Ending stock group price : " << g8_final_sqp_in << "€" << std::endl;
   //End of ostream
   std::cout << gzk_separator << std::endl;
}

/**
 * @see method which simulate gains over uprising stocks prices at sell
 * @param const double reference increment
 * @param const unsigned int reference cycle
 * @param const double reference gain limit
 * @return void
 */
void CNB::simGain(const double & p8r_inc, const unsigned int & pu4r_cycle, const double & p8r_gainLim)
{
   g8_out = g8_in;
   bool l1_pos = false;
   double l8_out, l8_tax, l8_realG, l8_start_realG;
   
   for(unsigned int lui_rot = 0; lui_rot < pu4r_cycle; g8_out += p8r_inc, ++lui_rot)
   {
      g8_gap = g8_out - g8_in;
      g8_gap_sqp = g8_gap * gu4_amt_in;
      g8_hot_out = (g8_out / 100) * g8k_coinbase_tx_sell;
      g8_hot_stock_out = g8_out;// + g8_hot_out;
      
      g8_sgp_out = g8_out * gu4_amt_in;
      
      g8_sgp_tax_out = (((g8_hot_stock_out * gu4_amt_in) / 100) * g8k_coinbase_tx_sell);
      
      g8_sgp_tx_out = g8_hot_stock_out * gu4_amt_in + g8_sgp_tax_out;
      g8_sqp_tx_val_out = g8_sgp_tx_out - g8_sgp_out;
      g8_sqp_fin_tx_val_out = g8_sqp_tx_val_out + g8k_coinbase_bkt_sepa_out;
      g8_final_sqp_out = g8_sgp_tx_out + g8k_coinbase_bkt_sepa_out;
      
      l8_tax = g8_sgp_tax_out + g8_sgp_tax_in + g8k_coinbase_bkt_sepa_out + g8k_coinbase_bkt_sepa_in;
      l8_realG = (g8_gap * gu4_amt_in) - l8_tax;
      
      if(l8_realG >= p8r_gainLim && !l1_pos)
      {
         l8_out = g8_out;
         l8_start_realG = l8_realG;
         ++l1_pos;
      }
      
      std::cout << gzk_separator << std::endl;
      std::cout << "from : " << g8_in << "€ to : " << g8_out << "€" << std::endl;
      std::cout << "gap is : " << g8_gap << "€ for 1 stock and : " << g8_gap_sqp << "€ for : " << gu4_amt_in << " stocks" << std::endl;
      std::cout << "tax (sell) : " << g8_sgp_tax_out << "€" << std::endl;
      std::cout << "tax (buy) : " << g8_sgp_tax_in << "€" << std::endl;
      std::cout << "tax (total with SEPA) : " << l8_tax << "€" << std::endl;
      std::cout << "gain : " << l8_realG << "€" << std::endl;
      std::cout << gzk_separator << std::endl;
   }
}

/**
 * @see function inCNB for computing real final stock group price after taxation of CNB
 * @param p8r_stockPrx const double reference stock price
 * @param p4r_stockNb const unsigned integer reference stock amount
 * @return g8_final_sqp_in Final Stock group price for INCOMING
 */
double CNB::inCNB(const double & p8r_stockPrx, const unsigned int & p4r_stockNb)
{
   g8_in = p8r_stockPrx;
   gu4_amt_in = p4r_stockNb;  
   //Stock computing
   g8_hot_in = (g8_in / 100) * g8k_coinbase_tx_buy;
   g8_hot_stock_in = g8_in;// + g8_hot_in;
   //Stock group computing
   g8_sgp_in = g8_in * gu4_amt_in;
   
   g8_sgp_tax_in = (((g8_hot_stock_in * gu4_amt_in) / 100) * g8k_coinbase_tx_buy);
   
   g8_sgp_tx_in = g8_hot_stock_in * gu4_amt_in + g8_sgp_tax_in;
   g8_sqp_tx_val_in = g8_sgp_tx_in - g8_sgp_in;
   g8_sqp_fin_tx_val_in = g8_sqp_tx_val_in + g8k_coinbase_bkt_sepa_in;
   g8_final_sqp_in = g8_sgp_tx_in + g8k_coinbase_bkt_sepa_in;
   
   return g8_final_sqp_in;
}

int main()
{  
   double l8_in, l8_out, l8_exp, l8_gainlim;
   unsigned int l4_amount, l4_cycle;

   CNB coinbase;
   /*
   std::cout << "enter stock price : " << std::endl; 
   std::cin >> l8_in;
   std::cout << "amount : " << std::endl;
   std::cin >> l4_amount;
   
   coinbase.inCNB(l8_in, l4_amount);
   coinbase.printData_in();
   
   std::cout << "expantion increment : " << std::endl;
   std::cin >> l8_exp;
   std::cout << "cycles : " << std::endl;
   std::cin >> l4_cycle;
   coinbase.simGain(l8_exp, l4_cycle, l8_gainlim);
   */
   std::cout << "check fast, stock amount : " << std::endl;
   std::cin >> l4_amount;
   std::cout << "in price : " << std::endl;
   std::cin >> l8_in;
   std::cout << "out price : " << std::endl;
   std::cin >> l8_out;
   std::cout << "gain : " << coinbase.fastGain(l8_in, l8_out, l4_amount) << "€" << std::endl;
   
   return 0;
}
