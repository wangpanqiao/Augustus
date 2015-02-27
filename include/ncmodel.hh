 /*****************************************************************************\
 * Filename : ncmodel.hh
 * Author   : Mario Stanke
 * Description: Model for non-protein-coding genes (nc)
 *
 *
 * Date       |   Author        |  Changes
 *------------|-----------------|----------------------------------------
 * 21.02.2015 | Mario Stanke    | creation of the file
 \******************************************************************************/

#ifndef _NCMODEL_HH
#define _NCMODEL_HH

#include "statemodel.hh"


/**
 * The nc model class.
 *
 * author Mario Stanke
 */
 
class NcModel : public StateModel {
public:
    NcModel();
    ~NcModel();

    StateType getStateType( ) const {
	return nctype;
    }

    /**
     * No training of parameters. Other parameters are reused.
     */
    void buildModel( const AnnoSequence* annoseq, int parIndex );
    void registerPars( Parameters* parameters);
  
    void printProbabilities   ( int zusNumber, BaseCount *bc, const char* suffix = NULL );
    void initAlgorithms       ( Matrix<Double>&, int);
    void viterbiForwardAndSampling(ViterbiMatrixType&, ViterbiMatrixType&, int, int, 
				   AlgorithmVariant, OptionListItem&) const;
    Double emiProbUnderModel  (int begin, int end) const;
    Double endPartEmiProb     (int begin, int end, int endOfBioExon) const;
    Double notEndPartEmiProb  (int begin, int end, int endOfBioExon, Feature *exonparts) const;
    void getEndPositions      ( int end, int &beginOfEndPart, int &endOfBioExon) const;
    Double tssupSeqProb       ( int left, int right, bool reverse) const;  
    Double tssProb            ( int tsspos) const;
    void precomputeTxEndProbs    ( );
    static void init();
    static void resetPars(){
	if (nccount == 0)
	    return;
	if (!haveSnippetProbs)
	    initSnippetProbs();
	initAlgorithmsCalled = false;
    }
    static void readAllParameters();
    static void resetModelCount(){nccount = 0;};
    
private:
    Double seqProb            ( int left, int right, bool reverse) const;
    void computeLengthDistributions( );
    Double longIntronProb(int internalBegin, int internalEnd) const;
    static void initSnippetProbs();

private:
    StateType              nctype;
    Integer                gweight;
    static Integer         nccount;
    static vector<Double>  lenDistInternal;       // length distribution of internal, initial and terminal
    static vector<Double>  lenDistSingle;         // length distribution of nc genes with a single exon
    static Boolean         hasLenDist;
    static bool            initAlgorithmsCalled;
    static bool            haveSnippetProbs;
    static SnippetProbs    *snippetProbs;
#    static double pUtr5Intron, pUtr3Intron, prUtr5Intron, prUtr3Intron;
    static int             boundSpacing; // without hints 5' and 3' transcript end only every ttsSpacing bases, for speed
    static vector<Double>  ttsProbPlus, ttsProbMinus;
    static vector<Double>  tssProbPlus, tssProbMinus;
};

class NcModelError : public ProjectError {
public:
    NcModelError(string msg) : ProjectError(msg) {}
};

#endif    //  _NCMODEL_HH
