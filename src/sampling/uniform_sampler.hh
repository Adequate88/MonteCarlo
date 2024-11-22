//
// Created by Alessandro Salo on 22.11.2024.
//

#ifndef UNIFORM_SAMPLER_HH
#define UNIFORM_SAMPLER_HH

class uniform {
    /** Implementation of the highest quality recommended generator. The constructor is called with
    an integer seed and creates an instance of the generator. The member functions int64, doub,
    and int32 return the next values in the random sequence, as a variable type indicated by their
    names. The period of the generator is  3:138  1057.
    */
    private:
        unsigned long long u{},v,w;
        inline unsigned long long int64();
        float minimum, maximum;
        void initialize(unsigned long long j);

    public:

        uniform(unsigned long long j); // Default sampler in range (0,1) with seed 'j'
        uniform(unsigned long long j, float a, float b); // Constructs sampler with range (a,b)
        double sample(); // Return random double-precision floating value within pre-defined interval


};
#endif //UNIFORM_SAMPLER_HH
