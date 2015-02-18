// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>
#include <cstdio>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};


#include "chainparamsseeds.h"

static const uint256 MAINNET_GENESIS("0x000e1594b025054f2cdba70fba0e74de5049d62ade0e0a65a2f990b6c8f19fd0");
static const uint256 TESTNET_GENESIS("0x000e2950b882a5785c7d962ccb4e507b02633ec29687d26687ba8b4cb810e39c");
static const uint256 REGNET_GENESIS("0x6fb42b979bd4f9efc8f736092e7c75dca739c3aeb12893db9f0bda9332cce317");


/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0, MAINNET_GENESIS)
        ( 1, uint256("0008417223dc102bfc3916c18cbc31df46f24ca42126d457d903efbb47a2538b")) //premine
        (16, uint256("0001df5d43ac5430e05addc67197765eb5672a24e8ed3936f1928035c75010c3")) //fair launch
        ;
static const Checkpoints::CCheckpointData data = { //just ignore all this stuff until we put in a real checkpoint
        &mapCheckpoints,
        0, // * UNIX timestamp of last checkpoint block
        0,   // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        0     // * estimated number of transactions per day after checkpoint
    };

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 0, TESTNET_GENESIS)
        ;
static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        0,
        0,
        0
    };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, REGNET_GENESIS)
        ;
static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

void  CChainParams::MineNewGenesisBlock()
{
    //This will output (to stdout) the code for a new genesis block when it is found
    genesis.nTime=time(NULL);
    genesis.nNonce=0;
    uint256 thash;
    while(1)
    {
        thash=genesis.GetHash();
        if (this->CheckProofOfWork(thash, genesis.nBits))
            break;
        if ((genesis.nNonce & 0xFF) == 0)
        {
            printf("nonce %08X: hash = %s\n",genesis.nNonce, thash.ToString().c_str());
        }
        ++genesis.nNonce;
        if (genesis.nNonce == 0)
        {
            printf("NONCE WRAPPED, incrementing time\n");
            ++genesis.nTime;
        }
    }
    printf("genesis.nTime = %u;\n",genesis.nTime);
    printf("genesis.nNonce = %u;\n",genesis.nNonce);
    printf("assert(genesis.hashMerkleRoot == uint256(\"0x%s\"));\n",genesis.hashMerkleRoot.ToString().c_str());
    printf("//genesis hash: 0x%s\n", genesis.GetHash().ToString().c_str());
    printf("//genesis hash: 0x%s\n", genesis.GetHash().ToString().c_str());
    exit(1);


}

//need a different implementation here that doesn't use error() and that doesn't use Params() since it isn't yet usable
bool CChainParams::CheckProofOfWork(uint256 hash, unsigned int nBits)
{
    bool fNegative;
    bool fOverflow;
    uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
        return false;

    return true;
}


class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb1;
        pchMessageStart[3] = 0xd0;
        vAlertPubKey = ParseHex("0496992EAFAB7117FAE9C8AA7B9E46333272260EA3F7DCDE170D3DFE52B2426E44A128B802EE91467547E5725616D20313EEE974E994662B0E7DAA7D3C28862C3D");
        nDefaultPort = 4411;
        bnProofOfWorkLimit = ~uint256(0) >> 12; //about 1.2 minutes for a single-core moderate processor PC
        nSubsidyHalvingInterval = 288 * 100; //288 blocks per day
        nMinerThreads = 0;
        nTargetTimespan = 5 * 60; // these have to be the same for digishield
        nTargetSpacing = 5 * 60;

        const char* pszTimestamp = "BTC block #343973 00000000000000000fb3c8734d61ffc9cf92908a81386ecc7b35882e05a84d90";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0") << OP_CHECKSIG;

        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
genesis.nTime = 1424229244;
genesis.nNonce = 369;
assert(genesis.hashMerkleRoot == uint256("0x62ba69e49dbc3a1d5ef1deabefe602c25d5dc3ca235508412b6453eb726d163b"));
//genesis hash: 0x000e1594b025054f2cdba70fba0e74de5049d62ade0e0a65a2f990b6c8f19fd0
//genesis hash: 0x000e1594b025054f2cdba70fba0e74de5049d62ade0e0a65a2f990b6c8f19fd0


        //MineNewGenesisBlock();
        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == MAINNET_GENESIS);

//        vSeeds.push_back(CDNSSeedData("earlz.net", "earlz.net"));
        vSeeds.push_back(CDNSSeedData("sup1.earlz.net", "sup1.earlz.net"));
        vSeeds.push_back(CDNSSeedData("sup2.earlz.net", "sup2.earlz.net"));
        vSeeds.push_back(CDNSSeedData("sup3.earlz.net", "sup3.earlz.net"));
        vSeeds.push_back(CDNSSeedData("sup1.supcoin.com", "sup1.supcoin.com"));
        vSeeds.push_back(CDNSSeedData("sup2.supcoin.com", "sup2.supcoin.com"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(48);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(43);
        base58Prefixes[SECRET_KEY] =     list_of(128);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultCheckMemPool = false;
        fAllowMinDifficultyBlocks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x1b;
        pchMessageStart[1] = 0x12;
        pchMessageStart[2] = 0x19;
        pchMessageStart[3] = 0x02;
        vAlertPubKey = ParseHex("04B1FBD698F99145DD8963F6625A8968B80F237E82E0F895B56CD271ACB1967B47A49026DEDB0C1101B946B50F9F74EDDB7B2D1ECDCA840F06C0775505ECA4E5D8");
        //testnet private key: 02B1FBD698F99145DD8963F6625A8968B80F237E82E0F895B56CD271ACB1967B47
        nDefaultPort = 14411;
        nMinerThreads = 0;
        nTargetTimespan = 5 * 60;
        nTargetSpacing = 5 * 60;

genesis.nTime = 1424229305;
genesis.nNonce = 569;
assert(genesis.hashMerkleRoot == uint256("0x62ba69e49dbc3a1d5ef1deabefe602c25d5dc3ca235508412b6453eb726d163b"));
//genesis hash: 0x000e2950b882a5785c7d962ccb4e507b02633ec29687d26687ba8b4cb810e39c
//genesis hash: 0x000e2950b882a5785c7d962ccb4e507b02633ec29687d26687ba8b4cb810e39c

        //MineNewGenesisBlock();
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == TESTNET_GENESIS);

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("testnetsup.earlz.net", "testnetsup.earlz.net"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(111);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(196);
        base58Prefixes[SECRET_KEY]     = list_of(239);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultCheckMemPool = false;
        fAllowMinDifficultyBlocks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xff;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xdf;
        nSubsidyHalvingInterval = 150;
        nMinerThreads = 1;
        nTargetTimespan = 5 * 60; 
        nTargetSpacing = 5 * 60;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();

genesis.nTime = 1424229368;
genesis.nNonce = 0;
assert(genesis.hashMerkleRoot == uint256("0x62ba69e49dbc3a1d5ef1deabefe602c25d5dc3ca235508412b6453eb726d163b"));
//genesis hash: 0x6fb42b979bd4f9efc8f736092e7c75dca739c3aeb12893db9f0bda9332cce317
//genesis hash: 0x6fb42b979bd4f9efc8f736092e7c75dca739c3aeb12893db9f0bda9332cce317



        //MineNewGenesisBlock();
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;

        assert(hashGenesisBlock == REGNET_GENESIS);

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultCheckMemPool = true;
        fAllowMinDifficultyBlocks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 18445;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultCheckMemPool = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval)  { nSubsidyHalvingInterval=anSubsidyHalvingInterval; }
    virtual void setDefaultCheckMemPool(bool afDefaultCheckMemPool)  { fDefaultCheckMemPool=afDefaultCheckMemPool; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {  fAllowMinDifficultyBlocks=afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams()
{
   assert(pCurrentParams);
   assert(pCurrentParams==&unitTestParams);
   return (CModifiableParams*)&unitTestParams;
}

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
