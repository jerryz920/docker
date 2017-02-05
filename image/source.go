package image

/// for simplicity we only have git, revision, file, and timestamp
type Source struct {
	Repo string `json:"repo,omitempty"`
	// hash of the revision
	Revision string `json:"revision,omitempty"`
	/// hash of dir
	Dir string `json:"dir,omitempty"`
	// Dockerfile used to build
	File string `json:"file,omitempty"`
	Time string `json:"time,omitempty"`
}
