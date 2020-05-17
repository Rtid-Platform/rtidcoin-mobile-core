//
//  BRChainParams.h
//
//  Created by Aaron Voisine on 1/10/18.
//  Copyright (c) 2020 rtidcoinwallet LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef BRChainParams_h
#define BRChainParams_h

#include "BRMerkleBlock.h"
#include "BRSet.h"
#include <assert.h>

typedef struct {
    uint32_t height;
    UInt256 hash;
    uint32_t timestamp;
    uint32_t target;
} BRCheckPoint;

typedef struct {
    const char * const *dnsSeeds; // NULL terminated array of dns seeds
    uint16_t standardPort;
    uint32_t magicNumber;
    uint64_t services;
    int (*verifyDifficulty)(const BRMerkleBlock *block, const BRSet *blockSet); // blockSet must have last 2016 blocks
    const BRCheckPoint *checkpoints;
    size_t checkpointsCount;
} BRChainParams;

static const char *BRMainNetDNSSeeds[] = {
    "explorer.rtid-platform.web.id.", "explorer.rtid-platform.web.id.", NULL
};

static const char *BRTestNetDNSSeeds[] = {
    "testnet.rtid-platform.web.id.", "testnet.rtid-platform.web.id.", NULL
};

// blockchain checkpoints - these are also used as starting points for partial chain downloads, so they must be at
// difficulty transition boundaries in order to verify the block difficulty at the immediately following transition
static const BRCheckPoint BRMainNetCheckpoints[] = {
     {      0, uint256("3f2253e50646bc794517f572ec666a6f3ba2df39fb79174b2c700d89db1e472e"), 1587892175, 0x20000fff },
     {  20160, uint256("fa376d5cf959da54d5161b78ca6194266d435894c7f38e501813242497a64a11"), 1588202619, 0x1e229f1e },
     {  40320, uint256("cc9f693f4e817ad495c8f30a4e0955833ac87af28c227fca98c44784e1ee8c0a"), 1588414285, 0x1e0b0e3a },
     {  60480, uint256("03c221a01b5586faae96ff898712985266a67340225ec47e88a8fe5b55c4dbe2"), 1588626003, 0x1e0e953a },
     {  80640, uint256("e5eef5a29f94b02cce4b336a5b4a890af336dca65792e2d0b9fd3ccda98df313"), 1588837831, 0x1e178bd9 }
};

static const BRCheckPoint BRTestNetCheckpoints[] = {
    {       0, uint256("000000000933ea01ad0ee984209779baaec3ced90fa3f408719526f8d77f4943"), 1296688602, 0x1d00ffff }
};

static int BRMainNetVerifyDifficulty(const BRMerkleBlock *block, const BRSet *blockSet)
{
    assert(block != NULL);
    assert(blockSet != NULL);

    return BRMerkleBlockVerifyDifficultyDarkGravityWave(block,blockSet);
}

static int BRTestNetVerifyDifficulty(const BRMerkleBlock *block, const BRSet *blockSet)
{
    return 1; // XXX skip testnet difficulty check for now
}

static const BRChainParams BRMainNetParams = {
    BRMainNetDNSSeeds,
    7899,       // standardPort
    0xd8dad78a, // magicNumber
    0,          // services
    BRMainNetVerifyDifficulty,
    BRMainNetCheckpoints,
    sizeof(BRMainNetCheckpoints)/sizeof(*BRMainNetCheckpoints)
};

static const BRChainParams BRTestNetParams = {
    BRTestNetDNSSeeds,
    20595,      // standardPort
    0xe72b6c97, // magicNumber
    0,          // services
    BRTestNetVerifyDifficulty,
    BRTestNetCheckpoints,
    sizeof(BRTestNetCheckpoints)/sizeof(*BRTestNetCheckpoints)
};

#endif // BRChainParams_h
