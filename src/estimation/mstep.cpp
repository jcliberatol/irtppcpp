#include <estimation/mstep.h>

namespace irtpp
{
  double mstep(model * m, Matrix<double> & z, m_parameter param)
  {
    // Define the vector that hold the function and the gradient
    std::vector<Func> functions;
    // Define the parameters
    ll_parameter p;

    // Store the function and the gradient
    functions.push_back(m->loglikelihood);
    functions.push_back(m->getGrad_Function());

    // Store the values
    p.r             = param.r;
    p.f             = param.f;
    //p.theta         = param.theta;
    p.gradient      = param.gradient;
    p.sum           = param.sum;
    p.probability   = m->getP_Function();
    p.boundary      = m->getBoundary_Function();

    double LLsum = 0;
    for (int i = 0; i < param.items; i++)
    {
      //std::cout << " Optimizing  "<< i << std::endl;
      // std::cout << i << " → \"a\" \"b\" \"c\"" << std::endl;
      // std::cout << z(i, 0) << " ";
      // std::cout << z(i, 1) << " ";
      // std::cout << z(i, 2) << std::endl;

      spgo::BFGS<Func, double *, ll_parameter> bfgs;
      bfgs.setParameterSize(param.param_size);
      p.index = i;
      //std::cout << "zmem: " << z.memory[i] << std::endl;
      bfgs.run(functions, z.memory[i], p);

      double * LL = m->loglikelihood(z.memory[i], p);
      //std::cout<<i<<" -  p : : "<<LL[0]<<std::endl;
        LLsum += LL[0];
      // std::cout << "\"a\" \"b\" \"c\"" << std::endl;

      // std::cout << z(i, 0) << " ";
      // std::cout << z(i, 1) << " ";
      // std::cout << z(i, 2) << std::endl;
    }
   return LLsum;
  }
}
