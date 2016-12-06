package builder

import (
	"os"

	"github.com/docker/docker/pkg/archive"
	"github.com/docker/docker/pkg/gitutils"
)

// MakeGitContext returns a Context from gitURL that is cloned in a temporary directory.
func MakeGitContext(gitURL string) (ModifiableContext, error) {
	root, err := gitutils.Clone(gitURL)
	if err != nil {
		return nil, err
	}

	c, err := archive.Tar(root, archive.Uncompressed)
	if err != nil {
		return nil, err
	}

	defer func() {
		// TODO: print errors?
		c.Close()
		os.RemoveAll(root)
	}()

	/// In the root we run 'git rev-parse --show-prefix HEAD', which will
	/// produce:
	///       work_dir
	///       sha1sum of current HEAD
	/// We use these information to create a special Label "Trust", which
	/// can't be modified by others via future changes. If an image is pulled,
	/// the docker daemon will also check if such "Trust" occurs, and it will
	/// merge them as Json dict of two keys: commit, dir
	if cwdHash, treeHash, err := gitutils.GitGetIdentity(); err != nil {
		return nil, err
	} else {
		if tarContext, err := MakeTarSumContext(c); err != nil {
			return nil, err
		} else {
			return MakeTrustedGitContext(tarContext, treeHash, cwdHash), nil
		}

	}

}

type trustedGitContext struct {
	ModifiableContext
	identityHash []byte
	cwdHash      []byte
}

func (tc *trustedGitContext) IdentityHash() []byte {
	return tc.identityHash
}

func (tc *trustedGitContext) CwdHash() []byte {
	return tc.cwdHash
}

func MakeTrustedGitContext(mc ModifiableContext,
	idHash []byte, cwdHash []byte) TrustedGitContext {
	return &trustedGitContext{mc, idHash, cwdHash}
}
