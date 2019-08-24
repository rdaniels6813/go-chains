package chains_test

import (
	"testing"

	"github.com/rdaniels6813/go-chains/chains"
	"github.com/stretchr/testify/assert"
)

func TestMacKeychain(t *testing.T) {
	k, err := chains.NewKeychain()
	assert.Nil(t, err)
	err = k.Set("go-chains", "unit_test", "unit_test_password")
	assert.Nil(t, err)
	v, err := k.Get("go-chains", "unit_test")
	assert.Nil(t, err)
	assert.Equal(t, "unit_test_password", v)
	err = k.Delete("go-chains", "unit_test")
	assert.Nil(t, err)
}
