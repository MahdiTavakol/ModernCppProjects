#include "test_communicator_helpers.hpp"


// a specific modification of the enginefixture
// which creates the engine first and add out of rank
// particles next ...
std::unique_ptr<Engine> build_engine_set_particles(const int& myId_,
    std::array<int, 3> ranks_,
    std::vector<double>& x_,
    std::vector<double>& v_,
    std::vector<double>& f_,
    std::vector<double>& r_,
    std::vector<double>& m_,
    int nGhosts)
    {
        std::unique_ptr<Communicator> communicator = std::make_unique<Communicator>(myId_, ranks_);

        // box dimensions
        array<double, 3> min = { -300.0,-300.0,-300.0 };
        array<double, 3> max = { 300.0, 300.0, 300.0 };
        std::unique_ptr<Box> box = std::make_unique<Box>(min, max);

        // mocked particles
        std::unique_ptr<Particles> mockedParticles =
            std::make_unique<MockedParticles>();


        // calculating the nmax values
        int nmax = x_.size() / 3;
        // converting the pointer back

        std::vector<std::unique_ptr<Ref>> inputs;
        inputs.push_back(std::move(mockedParticles));
        inputs.push_back(std::move(box));
        inputs.push_back(std::move(communicator));


        // creating the engine fixture
        EngineFixture engineFixture(inputs);
        // getting the engine_ptr
        auto engine_ptr = engineFixture.returnEngine();
        /* After the engine_ptr is created in the engineFixture
         * the init function is called form every Ref object
         * in the engine one of them is the particles which
         * removes out of range particles.
         * So we need to return a reference to particles
         * from the engine and then reset particles coordinates here
         */
         // returning the particles
        auto& particlesRef = engine_ptr->getParticlesForUpdate();
        // convering it to the mockedParticles since I need 
        // the reset_particles  function
        MockedParticles* mockedParticlesConv = dynamic_cast<MockedParticles*>(particlesRef.get());
        // checking the conversion
        if (mockedParticlesConv == nullptr)
            throw std::invalid_argument("Wrong type in the engine_ptr!");
        // resetting particle coordinates
        // copying the x, v, f, r, m values since we 
        // move the inputs to the particles by reference
        // so these would be null after moving into 
        // the mockedParticles object
        std::vector<double> xCopy = x_, vCopy = v_, fCopy = f_, rCopy = r_, mCopy = m_;
        mockedParticlesConv->resetParticles(nmax, nGhosts, xCopy, vCopy, fCopy, rCopy, mCopy);
        // returning the engine
        return engine_ptr;
    };


// building the engine .. it first creates particles and then builds the engine
// so each particle is in the correct rank in the returned engine
std::unique_ptr<Engine> set_particles_build_engine(
    const int& myId_,
    std::array<int, 3> ranks_,
    std::vector<double>& x_,
    std::vector<double>& v_,
    std::vector<double>& f_,
    std::vector<double>& r_,
    std::vector<double>& m_,
    double skin_) {
        // communicators
        std::unique_ptr<Communicator> communicator = std::make_unique<Communicator>(myId_, ranks_, skin_);
        // box dimensions
        array<double, 3> min = { -300.0,-300.0,-300.0 };
        array<double, 3> max = { 300.0, 300.0, 300.0 };
        // box
        std::unique_ptr<Box> box = std::make_unique<Box>(min, max);
        // copy since we used the std::move in the mockedParticles class
        std::vector<double> xCopy = x_, vCopy = v_, fCopy = f_, rCopy = r_, mCopy = m_;
        // creating the mockedParticles object
        int nmax = 20;
        int nlocal = 20;
        int nghosts = 0;
        std::unique_ptr<Particles> mockedParticles =
            std::make_unique<MockedParticles>(nmax, xCopy, vCopy, fCopy, rCopy, mCopy);
        // putting everything into the engine
        std::vector<std::unique_ptr<Ref>> inputs;
        inputs.push_back(std::move(mockedParticles));
        inputs.push_back(std::move(box));
        inputs.push_back(std::move(communicator));
        // creating the engine fixture
        EngineFixture engineFixture1(inputs);
        // getting the engine_ptr
        auto engine_ptr = engineFixture1.returnEngine();
        // returning the engine
        return engine_ptr;

    };


// checking the particle coordinates inside the communicator
void build_engine_check_communicator(
    const int& myId_,
    std::array<int, 3> ranks_,
    std::vector<double>& x_,
    std::vector<double>& v_,
    std::vector<double>& f_,
    std::vector<double>& r_,
    std::vector<double>& m_,
    std::vector<std::vector<double>> expectedXsVec_,
    std::vector<std::vector<double>> expectedVsVec_,
    std::vector<std::vector<double>> expectedFsVec_,
    std::vector<std::vector<double>> expectedRsVec_,
    std::vector<std::vector<double>> expectedMsVec_,
    const double& skin_
    ) {
        // creating the communicator object
        // each communicator gets its id from the MPI_Comm_rank before creation
        // in the real scenarios
        std::unique_ptr<Communicator> communicator =
            std::make_unique<Communicator>(myId_, ranks_, skin_);
        // box dimensions
        array<double, 3> min = { -300.0,-300.0,-300.0 };
        array<double, 3> max = { 300.0, 300.0, 300.0 };
        // box
        std::unique_ptr<Box> box = std::make_unique<Box>(min, max);
        // particles
        int nmax = 20;
        int nlocal = 20;
        // copies since we used the std::move in the mockedParticles class
        std::vector<double> xCopy = x_;
        std::vector<double> vCopy = v_;
        std::vector<double> fCopy = f_;
        std::vector<double> rCopy = r_;
        std::vector<double> mCopy = m_;
        // creating the mockedParticles object
        std::unique_ptr<Particles> mockedParticles =
            std::make_unique<MockedParticles>(nmax, xCopy, vCopy, fCopy, rCopy, mCopy);
        // putting everything into the engine
        std::vector<std::unique_ptr<Ref>> inputs;
        inputs.push_back(std::move(mockedParticles));
        inputs.push_back(std::move(box));
        inputs.push_back(std::move(communicator));
        // creating the engine fixture
        EngineFixture engineFixture(inputs);
        // getting the engine_ptr
        auto engine_ptr = engineFixture.returnEngine();
        // checking the ids of atoms
        auto& particles = engine_ptr->getParticlesForUpdate();
        // getting xs, vs, fs, ms and rs
        double* myXs = particles->getXData();
        double* myVs = particles->getVData();
        double* myFs = particles->getFData();
        double* myMs = particles->getMData();
        double* myRs = particles->getRData();
        particles->getNmaxNlocal(nmax, nlocal);
        // 
        auto& expXs = expectedXsVec_[myId_];
        auto& expVs = expectedVsVec_[myId_];
        auto& expFs = expectedFsVec_[myId_];
        auto& expRs = expectedRsVec_[myId_];
        auto& expMs = expectedMsVec_[myId_];
        // checking output
        REQUIRE_THAT(myXs, Array3DMatcher(expXs.data(), expXs.size() / 3, 1e-6));
        REQUIRE_THAT(myVs, Array3DMatcher(expVs.data(), expVs.size() / 3, 1e-6));
        REQUIRE_THAT(myFs, Array3DMatcher(expFs.data(), expFs.size() / 3, 1e-6));
        REQUIRE_THAT(myRs, Array1DMatcher(expRs.data(), expRs.size(), 1e-6));
        REQUIRE_THAT(myMs, Array1DMatcher(expMs.data(), expMs.size(), 1e-6));
    };



void checking_communicator(
    const int& myId_,
    Engine* engine_,
    std::vector<std::vector<double>> expectedXsVec_,
    std::vector<std::vector<double>> expectedVsVec_,
    std::vector<std::vector<double>> expectedFsVec_,
    std::vector<std::vector<double>> expectedRsVec_,
    std::vector<std::vector<double>> expectedMsVec_) {
        // the number of particles
        int nmax, nlocal;
        // getting the particlesRef
        auto& particlesRef = engine_->getParticlesForUpdate();
        // getting xs, vs, fs, rs and ms
        double* myXs = particlesRef->getXData();
        double* myVs = particlesRef->getVData();
        double* myFs = particlesRef->getFData();
        double* myRs = particlesRef->getRData();
        double* myMs = particlesRef->getMData();
        // getting the expected values
        auto& expXs = expectedXsVec_[myId_];
        auto& expVs = expectedVsVec_[myId_];
        auto& expFs = expectedFsVec_[myId_];
        auto& expRs = expectedRsVec_[myId_];
        auto& expMs = expectedMsVec_[myId_];
        // getting number of particles 
        particlesRef->getNmaxNlocal(nmax, nlocal);
        // checking the results
        REQUIRE_THAT(myXs, Array3DMatcher(expXs.data(), expXs.size() / 3, 1e-6));
        REQUIRE_THAT(myVs, Array3DMatcher(expVs.data(), expVs.size() / 3, 1e-6));
        REQUIRE_THAT(myFs, Array3DMatcher(expFs.data(), expFs.size() / 3, 1e-6));
        REQUIRE_THAT(myRs, Array1DMatcher(expRs.data(), expRs.size(), 1e-6));
        REQUIRE_THAT(myMs, Array1DMatcher(expMs.data(), expMs.size(), 1e-6));

    };